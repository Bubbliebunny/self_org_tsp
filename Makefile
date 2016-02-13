-include objects.mk


# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: selforg

# Tool invocations
CNOS_Assign_f: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "selforg" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets


-include ../makefile.targets
