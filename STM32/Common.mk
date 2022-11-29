
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      STM32/Common.mk

IMPORT_FOLDER = ../../../Import

KMS_LINKER_SCRIPTS = $(IMPORT_FOLDER)/LinkerScripts
KMS_MAKEFILES      = $(IMPORT_FOLDER)/Makefiles
KMS_STARTUP        = $(IMPORT_FOLDER)/Startup

INCLUDES = -I $(IMPORT_FOLDER)/Includes/STM -I $(IMPORT_FOLDER)/Includes

KMS_A_A     = $(IMPORT_FOLDER)/Libraries/$(CONFIG)_$(EMBEDDED)/KMS-A.a
KMS_STM32_A = $(IMPORT_FOLDER)/Libraries/$(CONFIG)_$(EMBEDDED)/KMS-STM32.a

SPI_SIM_B_A = ../Libraries/$(CONFIG)/SPI-Sim-B.a
