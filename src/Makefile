.RECIPEPREFIX := +
MAKEFLAGS := $(MAKEFLAGS) --no-print-directory
CXX := g++
NAME := ochr
FILES := Ochr.files



build := ../build/
lib := ../lib/
incl := ../include/

acxxflags := $(CXXFLAGS) -g -std=c++11 -I../../Effro/include -DDEBUG
aldflags := $(LDFLAGS)
aldlibs := $(LDLIBS) -lpthread

libf := $(lib)lib$(NAME).a

raw := $(shell cat $(FILES))
srcs := $(filter %.cpp,$(raw))

dpds := $(addprefix $(build),$(srcs:%.cpp=%.d))
objs := $(addprefix $(build),$(srcs:%.cpp=%.o))

hdrs := $(addprefix $(incl),$(filter %.h,$(raw)))



.PHONY: clean depclean all library doc

all: library
library: $(libf) $(hdrs)

include $(dpds)

$(libf): $(objs) $(dpds)
+@echo "Building library."
+@ar rc $@ $(objs)
+@ranlib $@

depend: $(dpds)
+@echo Done.

$(build)%.o : %.cpp
+@echo "Building object $(*F)."
+@$(CXX) $(acxxflags) -c $< -o $(build)$@

$(incl)%.h: %.h
+@echo "Linking $(*F)."
+@cp $< $@

$(build)%.d: %.cpp
+@echo "Building depend $(*F)."
+@echo -n "$@ $(build)" > $@
+@$(CXX) $(acxxflags) -MM $< >> $@

clean:
+@echo "Cleaning all objects."
+@rm -f $(build)*.o

depclean:
+@echo "Cleaning all dependency files."
+@rm -f $(build)*.d

doc:
+@echo "Making documentation."
+@doxygen
