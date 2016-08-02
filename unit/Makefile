.RECIPEPREFIX := +
MAKEFLAGS := $(MAKEFLAGS) --no-print-directory
CXX := g++
NAME := unit
FILES := Ochr.ut.files



build := ../build/unit/
run := ../run/

acxxflags := $(CXXFLAGS) -g -std=c++11 -I../../Gwirio/include -I../../Effro/include
aldflags := $(LDFLAGS) -L../../Gwirio/lib -L../../Effro/lib -L../lib
aldlibs := $(LDLIBS) -lgwirio -lochr -lpthread -leffro

progf := $(run)$(NAME)

raw := $(shell cat $(FILES))
srcs := $(filter %.cpp,$(raw))

dpds := $(addprefix $(build),$(srcs:%.cpp=%.d))
objs := $(addprefix $(build),$(srcs:%.cpp=%.o))



.PHONY: clean depclean all unit check base

all: unit
unit: $(progf)

include $(dpds)

$(progf): $(objs) $(dpds)
+@echo "Building unit test."
+@$(CXX) $(objs) $(aldflags) $(aldlibs) -o $@

depend: $(dpds)
+@echo Done.

$(build)%.o : %.cpp
+@echo "Building object $(*F)."
+@$(CXX) $(acxxflags) -c $< -o $@

$(build)%.d: %.cpp
+@echo "Building depend $(*F)."
+@echo -n "$@ $(build)" > $@
+@$(CXX) $(acxxflags) -MM $< >> $@


base:
+@echo "Building base code..."
+@cd ../src && $(MAKE)

check: $(progf)
+@echo "Running unit tests..."
+@cd $(run) && ./$(NAME)

clean:
+@echo "Cleaning all objects."
+@rm -f $(build)*.o

depclean:
+@echo "Cleaning all dependency files."
+@rm -f $(build)*.d

doc:
+@echo "Making documentation."
+@doxygen
