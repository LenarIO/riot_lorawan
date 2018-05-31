APPLICATION = lora_meetup_demo

# By default assume RIOT folder is inside this one
RIOTBASE ?= $(CURDIR)/RIOT

# Don't display build logs
QUIET ?= 1
#
# DEVELHELP enabled by default for all tests, set 0 to disable
DEVELHELP ?= 1

# By default use b-l072z-lrwan1
BOARD ?= b-l072z-lrwan1

# Require i2c
FEATURES_REQUIRED = periph_i2c

# Use by default sx1276 and region EU868
LORA_DRIVER ?= sx1276
LORA_REGION ?= EU868

# Semtech LoRaMAC pkg is required in order to user LoRaWAN
USEPKG += semtech-loramac

# Cayenne LPP pkg is required in order to send data to Cayenne app
USEPKG += cayenne-lpp

# Load the LoRa Module
USEMODULE += $(LORA_DRIVER)

# We need FMT for handling hex in keys
USEMODULE += fmt

# Use the isl29125 Color Module
USEMODULE += isl29125

# Import timers
USEMODULE += xtimer

# set default device parameters in case they are undefined
TEST_ISL29125_I2C     ?= I2C_DEV\(0\)
TEST_ISL29125_IRQ_PIN ?= GPIO_PIN\(0,0\)

# export parameters
CFLAGS += -DTEST_ISL29125_I2C=$(TEST_ISL29125_I2C)
CFLAGS += -DTEST_ISL29125_IRQ_PIN=$(TEST_ISL29125_IRQ_PIN)

# Tell LoRaMAC pkg which region should be used
CFLAGS += -DREGION_$(LORA_REGION)

# Required for building RIOT
include $(RIOTBASE)/Makefile.include
