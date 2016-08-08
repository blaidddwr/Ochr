.RECIPEPREFIX := +
MAKEFLAGS := $(MAKEFLAGS) --no-print-directory
CXX := g++
NAME := ochr
FILES := Ochr.files



build := ../build/
lib := ../lib/
incl := ../include/
ifndef IROOT
path := ../..
else
path := $(IROOT)
endif

acxxflags := $(CXXFLAGS) -g -std=c++11 -I../..
aldflags := $(LDFLAGS)
aldlibs := $(LDLIBS) -lpthread

libf := $(lib)lib$(NAME).a

raw := $(shell cat $(FILES))
srcs := $(filter %.cpp,$(raw))

dpds := $(addprefix $(build),$(srcs:%.cpp=%.d))
objs := $(addprefix $(build),$(srcs:%.cpp=%.o))



.PHONY: clean depclean all library doc install

all: library
library: $(libf) $(hdrs)

ifndef DEPIGNORE
include $(dpds)
endif

$(libf): $(objs) $(dpds)
+@echo "Building library."
+@ar rc $@ $(objs)
+@ranlib $@

depend: $(dpds)
+@echo Done.

$(build)%.o : %.cpp
+@echo "Building object $(*F)."
+@$(CXX) $(acxxflags) -c $< -o $@

$(build)%.d: %.cpp
+@echo "Building depend $(*F)"
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

install: $(libf)
+@echo "Installing module."
+@mkdir -p "$(path)/lib"
+@cp $(libf) "$(path)/lib"
