//-----------------------------------------------------------------------------
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include "platform/platform.h"

//-----------------------------------------------------------------------------

#include "gfx/vulkan/gfxVKDevice.h"
#include "gfx/vulkan/gfxVKVertexDecl.h"

void GFXVulkanVertexDecl::init(const GFXVertexFormat *format)
{
   AssertFatal(!mFormat, "");
   mFormat = format;
  
   //for(int i = 0; i < GFXVK->getNumVertexStreams(); ++i)
   //   _initVerticesFormat(i);   
}

void GFXVulkanVertexDecl::prepareVertexFormat() const
{
   AssertFatal(mFormat, "GFXVulkanVertexDecl - Not inited");
   /*if( GFXVK->mCapabilities.vertexAttributeBinding )
   {
      for ( U32 i=0; i < glVerticesFormat.size(); i++ )
      {
         const TVkVertexAttribData &vkElement = mVKVerticesFormat[i];
      
         glVertexAttribFormat( glElement.attrIndex, glElement.elementCount, glElement.type, glElement.normalized, (uintptr_t)glElement.pointerFirst );
         glVertexAttribBinding( glElement.attrIndex, glElement.stream );
      }

      updateActiveVertexAttrib( GFXGL->getOpenglCache()->getCacheVertexAttribActive() );

      return;
   }*/
}

void GFXVulkanVertexDecl::prepareBuffer_old(U32 stream, S32 mBuffer, S32 mDivisor) const
{
   PROFILE_SCOPE(GFXGLVertexDecl_prepare);
   AssertFatal(mFormat, "GFXGLVertexDecl - Not inited");

   /*if( GFXGL->mCapabilities.vertexAttributeBinding )
      return;*/   

	// Bind the buffer...
   //glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
   //GFXGL->getOpenglCache()->setCacheBinded(GL_ARRAY_BUFFER, mBuffer);

   // Loop thru the vertex format elements adding the array state...   
   for ( U32 i=0; i < mVKVerticesFormat.size(); i++ )
   {
      // glEnableVertexAttribArray are called and cache in GFXGLDevice::preDrawPrimitive

      const TVkVertexAttribData &e = mVKVerticesFormat[i];
      if(e.stream != stream)
         continue;
      
      /*glVertexAttribPointer(
         e.attrIndex,      // attribute
         e.elementCount,   // number of elements per vertex, here (r,g,b)
         e.type,           // the type of each element
         e.normalized,     // take our values as-is
         e.stride,         // stride between each position
         e.pointerFirst    // offset of first element
      );*/
      //glVertexAttribDivisor( e.attrIndex, mDivisor );
   }
}

void GFXVulkanVertexDecl::updateActiveVertexAttrib(U32 lastActiveMask) const
{
   AssertFatal(mVertexAttribActiveMask, "GFXGLVertexDecl::updateActiveVertexAttrib - No vertex attribute are active");

   //U32 lastActiveVerxtexAttrib = GFXGL->getOpenglCache()->getCacheVertexAttribActive();
   //if(mVertexAttribActiveMask == lastActiveVerxtexAttrib)
   //   return;

   //U32 forActiveMask = mVertexAttribActiveMask & ~lastActiveVerxtexAttrib;
   //U32 forDeactiveMask = ~mVertexAttribActiveMask & lastActiveVerxtexAttrib;
   /*for (int i = 0; i < Torque::VK_VertexAttrib_COUNT; ++i)
   {         
      if( BIT(i) & forActiveMask ) //if is active but not in last mask
         //glEnableVertexAttribArray(i);
      else if( BIT(i) & forDeactiveMask ) // if not active but in last mask
         //glDisableVertexAttribArray(i);
   }*/

   //GFXGL->getOpenglCache()->setCacheVertexAttribActive(mVertexAttribActiveMask);
}

void GFXVulkanVertexDecl::_initVerticesFormat2()
{
   /*for (U32 i = 0; i < GFXVK->getNumVertexStreams(); ++i)
   {
      _initVerticesFormat(i);
   }*/
}

void GFXVulkanVertexDecl::_initVerticesFormat(U32 stream)
{   
   U32 buffer = 0;
   U32 vertexSize = 0;

   for ( U32 i=0; i < mFormat->getElementCount(); i++ )
   {
      const GFXVertexElement &element = mFormat->getElement( i );

      if(element.getStreamIndex() != stream)
         continue;

      AssertFatal(!mFormat->hasBlendIndices() || !element.isSemantic(GFXSemantic::TEXCOORD) || (mFormat->hasBlendIndices() && element.isSemantic(GFXSemantic::TEXCOORD) && element.getSemanticIndex() < 2), "skinning with more than 2 used texcoords!");

      vertexSize += element.getSizeInBytes();
   }

   // Loop thru the vertex format elements adding the array state...
   U32 texCoordIndex = 0;
   for ( U32 i=0; i < mFormat->getElementCount(); i++ )
   {
      const GFXVertexElement &element = mFormat->getElement( i );

      if(element.getStreamIndex() != stream)
         continue;

      mVKVerticesFormat.increment();
      TVkVertexAttribData &vkElement = mVKVerticesFormat.last();
      vkElement.stream = element.getStreamIndex();

      if ( element.isSemantic( GFXSemantic::POSITION ) )
      {           
         //vkElement.attrIndex = Torque::VK_VertexAttrib_Position;
         vkElement.elementCount = element.getSizeInBytes() / 4;
         vkElement.normalized = false;
         //vkElement.type = GL_FLOAT;
         vkElement.stride = vertexSize;
         vkElement.pointerFirst = (void*)(uintptr_t)buffer;

         buffer += element.getSizeInBytes();
      }
      else if ( element.isSemantic( GFXSemantic::NORMAL ) )
      {
         //vkElement.attrIndex = Torque::VK_VertexAttrib_Normal;
         vkElement.elementCount = 3;
         vkElement.normalized = false;
         //vkElement.type = GL_FLOAT;
         vkElement.stride = vertexSize;
         vkElement.pointerFirst = (void*)(uintptr_t)buffer;

         buffer += element.getSizeInBytes();
      }
      else if ( element.isSemantic( GFXSemantic::TANGENT ) )
      {
         //vkElement.attrIndex = Torque::VK_VertexAttrib_Tangent;
         vkElement.elementCount = 3;
         vkElement.normalized = false;
         //vkElement.type = GL_FLOAT;
         vkElement.stride = vertexSize;
         vkElement.pointerFirst = (void*)(uintptr_t)buffer;

         buffer += element.getSizeInBytes();
      }
      else if ( element.isSemantic( GFXSemantic::TANGENTW ) )
      {
         //vkElement.attrIndex = Torque::VK_VertexAttrib_TangentW;
         vkElement.elementCount = element.getSizeInBytes()/4;
         vkElement.normalized = false;
         //vkElement.type = GL_FLOAT;
         vkElement.stride = vertexSize;
         vkElement.pointerFirst = (void*)(uintptr_t)buffer;

         buffer += element.getSizeInBytes();
      }
      else if ( element.isSemantic( GFXSemantic::BINORMAL ) )
      {
         //vkElement.attrIndex = Torque::VK_VertexAttrib_Binormal;
         vkElement.elementCount = 3;
         vkElement.normalized = false;
         //vkElement.type = GL_FLOAT;
         vkElement.stride = vertexSize;
         vkElement.pointerFirst = (void*)(uintptr_t)buffer;

         buffer += element.getSizeInBytes();
      }
      else if ( element.isSemantic( GFXSemantic::COLOR ) )
      {
         //vkElement.attrIndex = Torque::VK_VertexAttrib_Color;
         vkElement.elementCount = element.getSizeInBytes();
         vkElement.normalized = true;
         //vkElement.type = GL_UNSIGNED_BYTE;
         vkElement.stride = vertexSize;
         vkElement.pointerFirst = (void*)(uintptr_t)buffer;

         buffer += element.getSizeInBytes();
      }
      else if ( element.isSemantic( GFXSemantic::BLENDWEIGHT ) )
      {
         //vkElement.attrIndex = Torque::VK_VertexAttrib_BlendWeight0 + element.getSemanticIndex();
         vkElement.elementCount = 4;
         vkElement.normalized = false;
         //vkElement.type = GL_FLOAT;
         vkElement.stride = vertexSize;
         vkElement.pointerFirst = (void*)(uintptr_t)buffer;

         buffer += element.getSizeInBytes();
      }
      else if ( element.isSemantic( GFXSemantic::BLENDINDICES ) )
      {
         //vkElement.attrIndex = Torque::VK_VertexAttrib_BlendIndex0 + element.getSemanticIndex();
         vkElement.elementCount = 4;
         vkElement.normalized = false;
         //vkElement.type = GL_UNSIGNED_BYTE;
         vkElement.stride = vertexSize;
         vkElement.pointerFirst = (void*)(uintptr_t)buffer;

         buffer += element.getSizeInBytes();
      }
      else // Everything else is a texture coordinate.
      {
         String name = element.getSemantic();
         vkElement.elementCount = element.getSizeInBytes() / 4;
         texCoordIndex = getMax(texCoordIndex, element.getSemanticIndex());
         //vkElement.attrIndex = Torque::VK_VertexAttrib_TexCoord0 + texCoordIndex;
            
         vkElement.normalized = false;
         //vkElement.type = GL_FLOAT;
         vkElement.stride = vertexSize;
         vkElement.pointerFirst = (void*)(uintptr_t)buffer;

         buffer += element.getSizeInBytes();
         ++texCoordIndex;
      }

      //AssertFatal(!( mVertexAttribActiveMask & BIT(glElement.attrIndex) ), "GFXGLVertexBuffer::_initVerticesFormat - Duplicate vertex attrib index");
      //mVertexAttribActiveMask |= BIT(glElement.attrIndex);
   }

   mVertexSize[stream] = vertexSize;
   AssertFatal(vertexSize == buffer, "");
}
