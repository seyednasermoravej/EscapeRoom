/*
 * Copyright (c) 2022 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_gpio_595

/**
 * @file Driver for 595 SPI-based GPIO driver (extended to support 24 registers, 192 GPIOs).
 */

#include <errno.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

#define LOG_LEVEL CONFIG_GPIO_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(gpio_595);

/* Define the maximum number of SN74HC595 shift registers */
#define MAX_NUM_OF_595 24
/** Configuration data */
struct reg_595_config {
    /* gpio_driver_data needs to be first */
    struct gpio_driver_config common;

    struct spi_dt_spec bus;

    uint8_t ngpios;
};

/** Runtime driver data */
struct reg_595_drv_data {
    /* gpio_driver_data needs to be first */
    struct gpio_driver_config data;

    struct k_sem lock;

    uint8_t gpio_cache[MAX_NUM_OF_595]; /* Array to store the state of 192 GPIOs */
};

/**
 * @brief Write the GPIO states to the SN74HC595 shift registers
 *
 * @param dev Device struct of the 595
 * @param value Pointer to the GPIO state array
 *
 * @return 0 if successful, failed otherwise
 */
static int reg_595_write_registers(const struct device *dev, uint8_t *value) {
    const struct reg_595_config *config = dev->config;
    struct reg_595_drv_data *const drv_data = (struct reg_595_drv_data *const)dev->data;
    int ret = 0;

    uint8_t nwrite = config->ngpios / 8; /* Number of bytes to write */
    uint8_t buf[MAX_NUM_OF_595] = {0};
    memcpy(buf, value, MAX_NUM_OF_595);
    const struct spi_buf tx_buf[1] = {{

        // .buf = buf + (MAX_NUM_OF_595 - nwrite),
        .buf = buf,
        // .buf = &buf[MAX_NUM_OF_595 - nwrite],
        .len = nwrite,
    }};

    const struct spi_buf_set tx = {
        .buffers = tx_buf,
        .count = ARRAY_SIZE(tx_buf),
    };

    ret = spi_write_dt(&config->bus, &tx);
    if (ret < 0) {
        LOG_ERR("spi_write FAIL %d\n", ret);
        return ret;
    }

    memcpy(drv_data->gpio_cache, value, nwrite);
    return 0;
}

/**
 * @brief Setup the pin direction (input or output)
 *
 * @param dev Device struct of the 595
 * @param pin The pin number
 * @param flags Flags of pin or port
 *
 * @return 0 if successful, failed otherwise
 */
static int setup_pin_dir(const struct device *dev, uint32_t pin, int flags) {
    if ((flags & GPIO_OUTPUT) == 0U) {
        return -ENOTSUP; /* Only output pins are supported */
    }

    return 0;
}

/**
 * @brief Get the raw value of the GPIO port
 *
 * @param dev Device struct of the 595
 * @param value Pointer to store the raw value
 *
 * @return -ENOTSUP, as reading is not supported
 */
static int reg_595_port_get_raw(const struct device *dev, uint32_t *value) {
    return -ENOTSUP; /* SN74HC595 does not support reading GPIO states */
}

// static int reg_595_port_set_masked_raw(const struct device *dev, uint32_t mask, uint32_t value) {
int reg_595_port_set_masked_raw(const struct device *dev, uint8_t *mask, uint8_t *value) {
    struct reg_595_drv_data *const drv_data = (struct reg_595_drv_data *const)dev->data;
    uint8_t buf[MAX_NUM_OF_595]; /* Temporary buffer for GPIO states */
    int ret;
    const struct reg_595_config *config = dev->config;

    /* Can't do SPI bus operations from an ISR */
    if (k_is_in_isr()) {
        return -EWOULDBLOCK;
    }

    k_sem_take(&drv_data->lock, K_FOREVER);

    memcpy(buf, drv_data->gpio_cache, sizeof(buf));
    // for (int i = 0; i < MAX_NUM_OF_595; i++) {
    //     buf[i] = (buf[i] & ~(mask >> (i * 8))) | ((value >> (i * 8)) & (mask >> (i * 8)));
    // }
    for (int i = 0; i < config->ngpios / 8; i++) {
        buf[i] = (buf[i] & ~mask[i]) | (value[i] & mask[i]);
    }
    ret = reg_595_write_registers(dev, buf);

    k_sem_give(&drv_data->lock);
    return ret;
}

// static int reg_595_port_set_bits_raw(const struct device *dev, uint32_t mask) {
int reg_595_port_set_bits_raw(const struct device *dev, uint8_t *mask) {
    return reg_595_port_set_masked_raw(dev, mask, mask);
}

// static int reg_595_port_clear_bits_raw(const struct device *dev, uint32_t mask) {
int reg_595_port_clear_bits_raw(const struct device *dev, uint8_t *mask) {
    uint8_t buf[MAX_NUM_OF_595] = {0};
    return reg_595_port_set_masked_raw(dev, mask, buf);
}

// static int reg_595_port_toggle_bits(const struct device *dev, uint32_t mask) {
static int reg_595_port_toggle_bits(const struct device *dev, uint8_t *mask) {
    struct reg_595_drv_data *const drv_data = (struct reg_595_drv_data *const)dev->data;
    uint8_t buf[MAX_NUM_OF_595];
    int ret;

    /* Can't do SPI bus operations from an ISR */
    if (k_is_in_isr()) {
        return -EWOULDBLOCK;
    }

    k_sem_take(&drv_data->lock, K_FOREVER);

    memcpy(buf, drv_data->gpio_cache, sizeof(buf));
    for (int i = 0; i < MAX_NUM_OF_595; i++) {
        // buf[i] ^= (mask >> (i * 8));
        buf[i] ^= mask[i];
    }

    ret = reg_595_write_registers(dev, buf);

    k_sem_give(&drv_data->lock);
    return ret;
}

static int reg_595_pin_config(const struct device *dev, gpio_pin_t pin, gpio_flags_t flags) {
    int ret;

    /* Can't do SPI bus operations from an ISR */
    if (k_is_in_isr()) {
        return -EWOULDBLOCK;
    }

    if ((flags & GPIO_OPEN_DRAIN) != 0U) {
        return -ENOTSUP;
    };

    ret = setup_pin_dir(dev, pin, flags);
    if (ret) {
        LOG_ERR("595: error setting pin direction (%d)", ret);
    }

    if ((flags & GPIO_OUTPUT_INIT_LOW) != 0) {
        return reg_595_port_clear_bits_raw(dev, BIT(pin));
    } else if ((flags & GPIO_OUTPUT_INIT_HIGH) != 0) {
        return reg_595_port_set_bits_raw(dev, BIT(pin));
    }

    return ret;
}

static const struct gpio_driver_api api_table = {
    .pin_configure = reg_595_pin_config,
    .port_get_raw = reg_595_port_get_raw,
    .port_set_masked_raw = reg_595_port_set_masked_raw,
    .port_set_bits_raw = reg_595_port_set_bits_raw,
    .port_clear_bits_raw = reg_595_port_clear_bits_raw,
    .port_toggle_bits = reg_595_port_toggle_bits,
};

/**
 * @brief Initialization function of 595
 *
 * @param dev Device struct
 * @return 0 if successful, failed otherwise.
 */
static int reg_595_init(const struct device *dev) {
    const struct reg_595_config *const config = dev->config;
    struct reg_595_drv_data *const drv_data = (struct reg_595_drv_data *const)dev->data;

    if (!device_is_ready(config->bus.bus)) {
        LOG_ERR("Unable to get SPI bus device");
        return -ENODEV;
    }

    k_sem_init(&drv_data->lock, 1, 1);

    memset(drv_data->gpio_cache, 0, sizeof(drv_data->gpio_cache));
    return 0;
}

#define GPIO_PORT_PIN_MASK_FROM_NGPIOS(ngpios) ((gpio_port_pins_t)(((uint64_t)1 << (ngpios)) - 1U))

#define GPIO_PORT_PIN_MASK_FROM_DT_INST(inst)                                                      \
    GPIO_PORT_PIN_MASK_FROM_NGPIOS(DT_INST_PROP(inst, ngpios))

#define REG_595_INIT(n)                                                                            \
    static struct reg_595_config reg_595_##n##_config = {                                          \
        .common =                                                                                  \
            {                                                                                      \
                .port_pin_mask = GPIO_PORT_PIN_MASK_FROM_DT_INST(n),                               \
            },                                                                                     \
        .bus =                                                                                     \
            SPI_DT_SPEC_INST_GET(n, SPI_OP_MODE_MASTER | SPI_TRANSFER_MSB | SPI_WORD_SET(8), 0),   \
        .ngpios = DT_INST_PROP(n, ngpios),                                                         \
    };                                                                                             \
                                                                                                   \
    static struct reg_595_drv_data reg_595_##n##_drvdata = {};                                     \
                                                                                                   \
    /* This has to init after SPI master */                                                        \
    DEVICE_DT_INST_DEFINE(n, reg_595_init, NULL, &reg_595_##n##_drvdata, &reg_595_##n##_config,    \
                          POST_KERNEL, CONFIG_GPIO_595_INIT_PRIORITY, &api_table);

DT_INST_FOREACH_STATUS_OKAY(REG_595_INIT)