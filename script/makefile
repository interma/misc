FILE="./loop.sh"
exist=$(shell if [ -f $(FILE) ]; then echo "exist"; fi;)
all:
ifdef $(exist)
	@echo "exist"
else
	@echo "no exist"
endif
