//
//  FelixEngine.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/16/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__FelixEngine__
#define __FelixEngineIOS__FelixEngine__

#include "Resources.h"
#include "Pipeline.h"

#define APP_CONFIG "Config/main.xml" /**< Main configuration file for the engine to load. */ 
#define MAIN_PASS "main" /**< A default general pass name. */
#define SCREEN_PASS "screen" /**< A default pass name for drawing 2d elements. (hud, text, etc...) */
#define FINAL_FBO "final" /**< Name for the final system Frame Buffer. */

/**
 * Defines what device the engine is running on.
 */
enum DEV_TYPE {
   DEV_PHONE, /**< A phone like device. (iphone or ipod touch) */
   DEV_TABLET, /**< A tablet device. */
};

/**
 * An interface that defines a drawable item in the engine.
 */
struct Drawable {
   /**
    * An empty virtual destructor.
    */
   virtual ~Drawable() {}
   
   /**
    * Determines if this is visible.
    * @return true if visible or false if not.
    */
   virtual bool isVisible() const = 0;
   
   /**
    * Determines how to draw this.
    * @return Either blend or depth DRAW_TYPE.
    */
   virtual DRAW_TYPE getDrawType() const = 0;
   
   /**
    * Determines what pass this belongs to.
    * @return string name of the pass.
    */
   virtual std::string getPass() const = 0;
   
   /**
    * Determines the layer which is used to sort Drawables for drawing.
    * @return int layer.
    */
   virtual int getLayer() const = 0;
   
   
   /**
    * Sets how this is drawn.
    * @param type Either a blend or depth DRAW_TYPE.
    */
   virtual void setDrawType(DRAW_TYPE type) = 0;
   
   /**
    * Sets the visibilty.
    * @param vis A boolean value, true for visible and false for not visible.
    */
   virtual void setVisiblity(bool vis) = 0;
   
   /**
    * Sets the pass that this is associated with.
    * @param pass A string value for the associated pass.
    */
   virtual void setPass(const std::string &pass) = 0;
   
   /**
    * Sets the layer value.
    * @param layer An int value for the numbered layer.
    */
   virtual void setLayer(int layer) = 0;
   
   /**
    * Draws this to the screen if it is visible.
    */
   virtual void draw() const = 0;
};

/**
 * Interface for the Host display context
 */
struct HostDisplay {
   
   /**
    * An empty virtual destructor.
    */
   virtual ~HostDisplay() {}
   
   /**
    * Renders a single frame using the current Pipeline and passes.
    */
   virtual void render() = 0;
   
   /**
    * Clears the active color and depth buffers.
    * @param color Color to draw in the color buffer, the default is black.
    */
   virtual void clearContext(Color color = Color()) = 0;
   
   /**
    * Sets the current draw mode or type.
    * @param type Either blend or depth draw type enum.
    */
   virtual void setDrawType(DRAW_TYPE type) = 0;
   
   
   /**
    * Draws all of the Drawables in a pass.
    * @param pass string name of a pass to be drawn.
    */
   virtual void drawPass(const std::string &pass) = 0;
   
   /**
    * Adds a Drawable to a pass.
    * @param drawable Drawable pointer.
    * @param pass string name of a pass.
    */
   virtual void addToPass(const Drawable *drawable, const std::string &pass) = 0;
   
   /**
    * Empties or clears all of the Drawables from all of the passes.
    */
   virtual void emptyPasses() = 0;
   
   /**
    * Clears or removes all of the passes.
    */
   virtual void clearPasses() = 0;
   
   
   /**
    * Gets a pointer to Resource defined by a tag.
    * @param tag XMLTag that defines a Resource.
    * @return Resource pointer or NULL if it can not be determined from the tag.
    */
   virtual Resource* getResource(const XMLTag &tag) = 0;
   
   /**
    * Cleans up any resources not being currently used.
    */
   virtual void cleanUpResources() = 0;
   
   /**
    * Gets a Shader associated by a name, creates a new Shader if not avalible.
    * @param name string name of the Shader.
    * @return Shader pointer.
    */
   virtual const Shader* getShader(const std::string &name) = 0;

   /**
    * Gets a Texture associated by a name, creates a new Texture if not avalible.
    * @param name string name of the Texture.
    * @return Texture pointer.
    */
   virtual const Texture* getTexture(const std::string &name) = 0;
   
   /**
    * Gets a Mesh associated by a name, creates a new Mesh if not avalible.
    * @param name string name of the Mesh.
    * @return Mesh pointer.
    */
   virtual const Mesh* getMesh(const std::string &name) = 0;
   
   /**
    * Gets a FrameBuff associated by a name, creates a new FrameBuff if not avalible.
    * @param name string name of the FrameBuff.
    * @return FrameBuffer pointer.
    */
   virtual const FrameBuff* getFrameBuff(const std::string &name) = 0;
   
   
   /**
    * Sets ShaderData to the Shader with the name, creates a new Shader if not avalible.
    * @param name string name of the Shader.
    * @param data ShaderData to be set to the Shader.
    */
   virtual void setShaderData(const std::string &name, const ShaderData &data) = 0;
   
   /**
    * Sets TextureData to the Texture with the name, creates a new Texture if not avalible.
    * @param name string name of the Texture.
    * @param data TextureData to set to the Texture.
    */
   virtual void setTextureData(const std::string &name, const TextureData &data) = 0;
   
   /**
    * Sets MeshData to the Mesh with the name, creates a new Mesh if not avalible.
    * @param name string name of the Mesh.
    * @param data MeshData to be set to Mesh.
    */
   virtual void setMeshData(const std::string &name, const MeshData &data) = 0;
   
   /**
    * Sets FrameBuffData to the FrameBuff with the name, creates a new FrameBuff if not avalible.
    * @param name string name of the FrameBuff.
    * @param data FrameBuffData to set the FrameBuff.
    */
   virtual void setFrameBuffData(const std::string &name, const FrameBuffData &data) = 0;
   
   
   /**
    * Set the active shader for the context.
    * @param sh Pointer to a Shader.
    */
   virtual void setCurShader(const Shader *sh) = 0;
   
   /**
    * Sets Uniform values to the active shader of the context.
    * @param uniforms Uniforms to be set to the Shader.
    */
   virtual void setCurUniforms(const Uniforms *uniforms) = 0;
   
   /**
    * Sets Attribute values to the active shader of the context.
    * @param attributes Attributes to be set to the Shader.
    */
   virtual void setCurAttributes(const Attributes *attributes) = 0;
   
   
   /**
    * Sets the active Render Pipeline for HostDisplay to use.
    * @param pipeline [in] Pointer to a Pipeline.
    */
   virtual void setCurPipeline(Pipeline *pipeline = 0) = 0;
   
   
   /**
    * Adds a Uniform key value pair to a collection of Uniforms associated with a pass.
    * @param name string for the key of the Uniform value.
    * @param uniform Uniform to be added.
    * @param pass string for the name of the pass.
    */
   virtual void addPassUniform(const std::string &name, const Uniform &uniform, const std::string &pass) = 0;
   
   /**
    * Removes a Uniform value associated with the name/key on a pass.
    * @param name string name of the Uniform value.
    * @param pass string name of the pass.
    */
   virtual void removePassUniform(const std::string &name, const std::string &pass) = 0;
   
   /**
    * Clears or removes all of the Uniform values for a pass.
    * @param pass string name of the pass.
    */
   virtual void clearPassUniforms(const std::string &pass) = 0;
};



/**
 * An interface for working with a host audio context.
 */
struct HostAudio {
   /**
    * An empty virtual destructor.
    */
   virtual ~HostAudio() {}
   
   /**
    * Gets a pointer to resource defined by a tag.
    * @param tag XMLTag that defines a resource.
    * @return Pointer to a Resource or NULL if it can not be determined from the tag.
    */
   virtual Resource* getResource(const XMLTag &tag) = 0;
   
   /**
    * Cleans up any Resources not being used.
    */
   virtual void cleanUpResources() = 0;
};



/**
 * An interface for working with the host file system
 */
struct HostFileSystem {
   /**
    * An empty virtual destructor.
    */
   virtual ~HostFileSystem() {}
   
   /**
    * Loads a textfile into a string.
    * @param path String value for the path to the text file.
    * @return string contents of the file or empty if unable to open the file.
    */
   virtual std::string loadTxt(const std::string &path) const = 0;
   
   /**
    * Creates an XMLTag from an xml file.
    * @param path string path to the xml file.
    * @return XMLTag pointer or NULL if unable to open the file.
    */
   virtual XMLTag* loadXML(const std::string &path) const = 0;
   
   /**
    * Creates Texture Data from an image file.
    * @param path string path to the image file.
    * @return TextureData pointer or NULL if unable to open the file.
    */
   virtual TextureData* loadTexture(const std::string &path) const = 0;
   
   /**
    * Creates Mesh Data from a 3d object file.
    * @param path string path to the 3d object file.
    * @param data [in,out] map to add MeshData to.
    * @return true if able to open and read the file or false if not.
    */
   virtual bool loadMeshes(const std::string &path, std::map<std::string, MeshData*> *data) const = 0;
};


/**
 * Base class that represents the host system.
 */
class Host: public Entity {
public:
   
   /**
    * Gets the device type of the Host.
    * @return Enum for the device.
    */
   inline DEV_TYPE getDeviceType()  const {return _device;}
   
   
   
   /**
    * Gets the current screen size of the Host.
    * @return ivec2
    */
   inline ivec2 getScreenSize()  const {return _size;}
   
   /**
    * Gets the scale of the screen. (2.0f for Retina and 1.0f for Non-Retina Displays)
    * @return float scale.
    */
   inline float getScreenScale() const {return _scale;}
   
   
   
   /**
    * Gets the HostDisplay.
    * @return HostDisplay pointer.
    */
   inline HostDisplay *getDisplay() const {return _display;}
   
   /**
    * Gets the HostAudio.
    * @return HostAudio pointer.
    */
   inline HostAudio *getAudio() const {return _audio;}
   
   /**
    * Gets the HostFileSystem.
    * @return HostFileSystem pointer.
    */
   inline HostFileSystem *getFileSystem() const {return _fileSys;}
   
   
   
   /**
    * Gets the resource for the tag from either the Display or Audo Context.
    * @param tag XMLTag that specifies the Resource to get.
    * @return Resource pointer or NULL if it can not be found.
    */
   inline Resource* getResource(const XMLTag &tag) {
      if (tag == "Shader" || tag == "Texture" || tag == "Mesh" || tag == "FrameBuff")
         return _display->getResource(tag);
      else if (tag == "Sound")
         return _audio->getResource(tag);
      return NULL;
   }
   
   /**
    * Clean up the unused Resources in HostDisplay and HostAudio.
    */
   inline void cleanUpResources() {
      _display->cleanUpResources();
      _audio->cleanUpResources();
   }
   
   /**
    * Gets the Host singlton instance.
    * @return Host pointer.
    */
   static Host* GetHost() {return Instance;}
   
protected:
   
   /**
    * The Host Constructor.
    * @param dev enum for the device.
    * @param size ivec2 of the screen size.
    * @param scale float for the screen scale.
    */
   Host(DEV_TYPE dev, ivec2 size, float scale):
   _device(dev), _size(size), _scale(scale), _display(0), _audio(0), _fileSys(0){
      Instance = this;
   }
   
   /**
    * Destructor deletes the HostDisplay, HostAudio, and HostFileSystem.
    */
   virtual ~Host() {
      delete _display;
      delete _audio;
      delete _fileSys;
   }
   
   DEV_TYPE _device; /**< device type of the host. */
   ivec2 _size; /**< screen size */
   float _scale; /**< screen scale */
   
   HostDisplay *_display; /**< HostDisplay pointer */
   HostAudio *_audio; /**< HostAudio pointer */
   HostFileSystem *_fileSys; /**< HostFileSystem pointer */
   
   static Host *Instance; /**< Host singleton instance */
};


#endif /* defined(__FelixEngineIOS__FelixEngine__) */
