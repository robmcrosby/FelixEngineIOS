
CC = g++
LD = g++

OPATH = .generated
BINDIR = bin

LIBDIR = tbb_osx

BINARIES = $(BINDIR)/RayTracer

CCFLAGS = -c
LDFLAGS = -ltbb -L $(LIBDIR)

CCFLAGS += -O3

OBJFILES = $(OPATH)/ImageLoader.o $(OPATH)/main.o $(OPATH)/PerspCamera.o \
$(OPATH)/Plane.o $(OPATH)/PointLight.o $(OPATH)/PovSceneBuilder.o \
$(OPATH)/RayScheduler.o $(OPATH)/RayTracer.o $(OPATH)/RenderBuffer.o \
$(OPATH)/Scene.o $(OPATH)/Shader.o $(OPATH)/ShaderBounce.o \
$(OPATH)/ShaderLight.o $(OPATH)/ShaderSurface.o $(OPATH)/Sphere.o \
$(OPATH)/TgaImageLoader.o $(OPATH)/RayNode.o $(OPATH)/Triangle.o \
$(OPATH)/Mesh.o $(OPATH)/BVHTree.o $(OPATH)/Distributions.o \
$(OPATH)/ImageBuffer.o $(OPATH)/ShaderGI.o $(OPATH)/ShaderAO.o

all: $(OPATH) $(BINDIR) $(BINARIES)
	-cp $(LIBDIR)/libtbb.dylib $(BINDIR)/libtbb.dylib

$(OPATH):
	mkdir $(OPATH)

$(BINDIR):
	mkdir $(BINDIR)
	mkdir $(BINDIR)/Images
	mkdir $(BINDIR)/SampleScenes
	cp -f Images/* $(BINDIR)/Images
	cp -f SampleScenes/* $(BINDIR)/SampleScenes

$(BINDIR)/RayTracer: $(OBJFILES)
	$(LD) $(LDFLAGS) $(OBJFILES) -o $@

$(OPATH)/%.o: %.cpp
	$(CC) $(CCFLAGS) $< -o $@

final: all
	-cd $(BINDIR); \
	echo "final1.pov" > final_times.txt; \
	( time ./RayTracer -p -i SampleScenes/final1.pov -o final1.tga ) 2>> final_times.txt; \
	echo "valentine2.pov" >> final_times.txt; \
	( time ./RayTracer -p -i SampleScenes/valentine2.pov -o valentine.tga ) 2>> final_times.txt; \
	echo "recurse_simp2.pov" >> final_times.txt; \
        ( time ./RayTracer -p -i SampleScenes/recurse_simp2.pov -o recurse_simp2.tga ) 2>> final_times.txt; \
	echo "balls2.pov" >> final_times.txt; \
        ( time ./RayTracer -p -i SampleScenes/balls2.pov -o balls2.tga ) 2>> final_times.txt

run: all
	-cd $(BINDIR); \
	echo "simple_refract.pov" > times.txt; \
	( time ./RayTracer -i SampleScenes/simple_refract.pov -o simple_refract.tga ) 2>> times.txt; \
	echo "\n\nrecurse_simp.pov" >> times.txt; \
        ( time ./RayTracer -i SampleScenes/recurse_simp.pov -o recurse_simp.tga ) 2>> times.txt; \
	echo "\n\nrecurses.pov" >> times.txt; \
        ( time ./RayTracer -i SampleScenes/recurses.pov -o recurses.tga ) 2>> times.txt; \
	echo "\n\nsimple_tri.pov" >> times.txt; \
        ( time ./RayTracer -i SampleScenes/simple_tri.pov -o simple_tri.tga ) 2>> times.txt; \
	echo "\n\nugly_part.pov" >> times.txt; \
        ( time ./RayTracer -i SampleScenes/ugly_part.pov -o ugly_part.tga ) 2>> times.txt; \
        echo "\n\nmarbles.pov" >> times.txt; \
        ( time ./RayTracer -i SampleScenes/marbles.pov -o marbles.tga ) 2>> times.txt; \
	echo "\n\nballs2.pov" >> times.txt; \
	( time ./RayTracer -i SampleScenes/balls2.pov -o balls2.tga ) 2>> times.txt; \
	echo "\n\ngnarly.pov" >> times.txt; \
	( time ./RayTracer -i SampleScenes/gnarly.pov -o gnarly.tga ) 2>> times.txt; \
	echo "\n\nRCcornell.pov" >> times.txt; \
	( time ./RayTracer -i SampleScenes/RCcornell.pov -o RCcornell.tga ) 2>> times.txt

clean:
	-rm -rf $(BINARIES)
	-rm -rf $(BINDIR)
	-rm -rf $(OPATH)
