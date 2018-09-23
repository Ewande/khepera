CXX = g++ # C++ compiler
CXXFLAGS = -c # C++ flags
LDFLAGS = -shared -fPIC # linking flags
RM = rm -f
TARGET_LIB = SimulationServer.so
SRC_PATH = ./SimulationServer

SRCS = $(shell find $(SRC_PATH)/Simulation -name *.cpp)
SRCS += $(SRC_PATH)/DllInterface.cpp
OBJS = $(SRCS:.cpp=.o)
print-%  : ; @echo $* = $($*)


.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CXX) ${LDFLAGS} -o $@ $^

-include $(OBJS:.o=.d)

$(OBJS): %.o:%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
	$(CXX) $(CXXFLAGS) -MM $< >$*.d


.PHONY: clean
clean:
	-$(RM) $(TARGET_LIB) $(OBJS) $(SRCS:.cpp=.d)
