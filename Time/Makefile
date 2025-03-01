#General options
WX = 3.2
GTK = 3

# General compiler options
CXX = g++
CFLAGS = -Wall -std=c++11

# Includes
INCL = -ICommon/ -I/usr/local/lib/wx/include/gtk$(GTK)-unicode-$(WX) -I/usr/local/include/wx-$(WX)

# Release flags
RELCFLAGS = -DNDEBUG -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread

# Debug flags
DBGCFLAGS = -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread

# Libraries
WXLFLAGS = -lwx_gtk$(GTK)u_xrc-$(WX) -lwx_gtk$(GTK)u_webview-$(WX) -lwx_gtk$(GTK)u_stc-$(WX) -lwx_gtk$(GTK)u_richtext-$(WX) -lwx_gtk$(GTK)u_ribbon-$(WX) -lwx_gtk$(GTK)u_propgrid-$(WX) -lwx_gtk$(GTK)u_aui-$(WX) -lwx_gtk$(GTK)u_gl-$(WX) -lwx_gtk$(GTK)u_html-$(WX) -lwx_gtk$(GTK)u_qa-$(WX) -lwx_gtk$(GTK)u_adv-$(WX) -lwx_gtk$(GTK)u_core-$(WX) -lwx_baseu_xml-$(WX) -lwx_baseu_net-$(WX) -lwx_baseu-$(WX)
LDFLAGS = $(WXLFLAGS)
LDDFLAGS = $(WXLFLAGS)

# Paths & files
# !!! >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
SRCPATH = Common
SRC = main.cpp my_dt_format.cpp

OBJS = $(SRC:.cpp=.o)
EXE = wxWidgetsApp

RELDIR = Release_linux/obj
RELBIN = Release_linux
RELEXE = $(RELBIN)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))

DBGDIR = Debug_linux/obj
DBGBIN = Debug_linux
DBGEXE = $(DBGBIN)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))

All: Clean Prepare Release Debug
Remake: remakeRelease remakeDebug
remakeRelease: cleanRelease Release
remakeDebug: cleanDebug Debug
Prepare: prepareRelease prepareDebug

# Release
Release: prepareRelease doRelease

prepareRelease:
	mkdir -p $(RELDIR)
	mkdir -p $(RELBIN)

doRelease: $(RELOBJS)
	$(CXX) $(RELOBJS) $(LDFLAGS) -o $(RELEXE)

$(RELOBJS): $(RELDIR)/%.o: $(SRCPATH)/%.cpp
	$(CXX) -c $< $(CFLAGS) $(RELCFLAGS) $(INCL) -o $@

# Debug
Debug: prepareDebug doDebug

prepareDebug:
	mkdir -p $(DBGDIR)
	mkdir -p $(DBGBIN)

doDebug: $(DBGOBJS)
	$(CXX) $(DBGOBJS) $(LDDFLAGS) -o $(DBGEXE)

$(DBGOBJS): $(DBGDIR)/%.o: $(SRCPATH)/%.cpp
	$(CXX) -c $< $(CFLAGS) $(DBGCFLAGS) $(INCL) -o $@

# Clean
Clean: cleanRelease cleanDebug

cleanRelease:
	rm -f -v $(RELDIR)/*.o $(RELEXE)

cleanDebug:
	rm -f -v $(DBGDIR)/*.o $(DBGEXE)

# Other
.PHONY: All doRelease doDebug Prepare prepareRelease prepareDebug Clean cleanRelease cleanDebug Remake remakeRelase remakeDebug

# DO NOT DELETE
