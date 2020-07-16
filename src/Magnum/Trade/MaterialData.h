#ifndef Magnum_Trade_MaterialData_h
#define Magnum_Trade_MaterialData_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019,
                2020 Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

/** @file
 * @brief Class @ref Magnum::Trade::MaterialData
 * @m_since_latest
 */

#include <Corrade/Containers/StringView.h>
#include <Corrade/Containers/Array.h>

#include "Magnum/Magnum.h"
#include "Magnum/Trade/visibility.h"

namespace Magnum { namespace Trade {

/**
@brief Material attribute name
@m_since_latest

*/
enum class MaterialAttribute {
    /**
     * Alpha mask, @ref MaterialAttributeType::Float. If set together with
     * @ref MaterialAttributeType::AlphaBlend, blending is preferred, however
     * renders can fall back to alpha-masked rendering.
     */
    AlphaMask,

    /**
     * Alpha blending, @ref MaterialAttributeType::Bool. If @cpp true @ce, the
     * material is expected to be rendered with blending enabled and in correct
     * depth order. If @cpp false @ce or not present, the material should be
     * treated as opaque.
     * @see @ref MaterialAttribute::AlphaMask
     */
    AlphaBlend,

    /**
     * Double sided, @ref MaterialAttributeType::Bool. If not present, the
     * default value is @cpp false @ce.
     */
    DoubleSided,

    /**
     * Ambient color for Phong materials, @ref MaterialAttributeType::Vector4.
     * If @ref MeshAttribute::AmbientTexture is present as well, these two are
     * multiplied together.
     */
    AmbientColor,

    /**
     * Ambient texture index for Phong materials,
     * @ref MaterialAttributeType::UnsignedInt. If
     * @ref MeshAttribute::AmbientColor is present as well, these two are
     * multiplied together.
     */
    AmbientTexture,

    /**
     * Ambient texture coordinate set index for Phong materials,
     * @ref MaterialAttributeType::UnsignedInt. Either this or
     * @ref MaterialAttribute::CoordinateSet can be present.
     */
    AmbientCoordinateSet,

    /**
     * Ambient texture transformation matrix for Phong materials,
     * @ref MaterialAttributeType::Matrix3x3. Either this or
     * @ref MaterialAttribute::TextureMatrix can be present.
     */
    AmbientTextureMatrix,

    /**
     * Diffuse color for Phong materials, @ref MaterialAttributeType::Vector4.
     * If @ref MeshAttribute::DiffuseTexture is present as well, these two are
     * multiplied together.
     */
    DiffuseColor,

    /**
     * Diffuse texture index for Phong materials,
     * @ref MaterialAttributeType::UnsignedInt. If
     * @ref MeshAttribute::DiffuseColor is present as well, these two are
     * multiplied together.
     */
    DiffuseTexture,

    /**
     * Diffuse texture coordinate set index for Phong materials,
     * @ref MaterialAttributeType::UnsignedInt. Either this or
     * @ref MaterialAttribute::CoordinateSet can be present.
     */
    DiffuseCoordinateSet,

    /**
     * Diffuse texture transformation matrix for Phong materials,
     * @ref MaterialAttributeType::Matrix3x3. Either this or
     * @ref MaterialAttribute::TextureMatrix can be present.
     */
    DiffuseTextureMatrix,

    /**
     * Specular color for Phong materials, @ref MaterialAttributeType::Vector4.
     * If @ref MeshAttribute::SpecularTexture is present as well, these two are
     * multiplied together.
     */
    SpecularColor,

    /**
     * Specular texture index for Phong materials,
     * @ref MaterialAttributeType::UnsignedInt. If
     * @ref MeshAttribute::SpecularColor is present as well, these two are
     * multiplied together.
     */
    SpecularTexture,

    /**
     * Specular texture coordinate set indexf or Phong materials,
     * @ref MaterialAttributeType::UnsignedInt. Either this or
     * @ref MaterialAttribute::CoordinateSet can be present.
     */
    SpecularCoordinateSet,

    /**
     * Specular texture transformation matrix for Phong materials,
     * @ref MaterialAttributeType::Matrix3x3. Either this or
     * @ref MaterialAttribute::TextureMatrix can be present.
     */
    SpecularTextureMatrix,

    /**
     * Tangent-space normal map texture index,
     * @ref MaterialAttributeType::UnsignedInt.
     */
    NormalTexture,

    /**
     * Normal texture coordinate set index,
     * @ref MaterialAttributeType::UnsignedInt. Either this or
     * @ref MaterialAttribute::CoordinateSet can be present.
     */
    NormalCoordinateSet,

    /**
     * Normal texture transformation matrix,
     * @ref MaterialAttributeType::Matrix3x3. Either this or
     * @ref MaterialAttribute::TextureMatrix can be present.
     */
    NormalTextureMatrix,

    /**
     * Common texture coordinate set index for all textures,
     * @ref MaterialAttributeType::UnsignedInt. Either this or (a subset of)
     * @ref MaterialAttribute::AmbientCoordinateSet /
     * @ref MaterialAttribute::DiffuseCoordinateSet /
     * @ref MaterialAttribute::SpecularCoordinateSet /
     * @ref MaterialAttribute::NormalCoordinateSet should be present.
     */
    CoordinateSet,

    /**
     * Common texture transformation matrix for all textures,
     * @ref MaterialAttributeType::Matrix3x3. Either this or (a subset of)
     * @ref MaterialAttribute::AmbientTextureMatrix /
     * @ref MaterialAttribute::DiffuseTextureMatrix /
     * @ref MaterialAttribute::SpecularTextureMatrix /
     * @ref MaterialAttribute::NormalTextureMatrix should be present.
     */
    TextureMatrix,

    /**
     * Shininess value for Phong materials, @ref MaterialAttributeType::Float.
     */
    Shininess
};

/**
@brief Material attribute type
@m_since_latest

As each @ref MaterialAttributeData instance has a fixed size anyway, the set of
available types is deliberately reduced to full types, thus no 8-, 16- or
half-float types. Moreover, @ref Double types are currently not supported
either as there isn't currently seen any need for extended precision.

@m_class{m-block m-warning}

@par Max representable data size
    With the current design, @ref MaterialAttributeData is 64 bytes and in
    order to fit a type identifier and a string attribute name of a reasonable
    length, the maximum data size is capped to 48 bytes. This means
    @ref Matrix4x4 isn't listed among supported types, but it shouldn't be a
    problem in practice --- ever an arbitrary color correction matrix is just
    3x4 values with the bottom row being always @f$ (0 0 0 1) @f$. This
    restriction might get lifted eventually.

@see @ref MaterialAttribute, @ref MaterialAttributeData
*/
enum class MaterialAttributeType: UnsignedByte {
    /* Zero used for an invalid value */

    Bool = 1,       /**< @cpp bool @ce */

    Float,          /**< @ref Float */
    UnsignedInt,    /**< @ref UnsignedInt */
    Int,            /**< @ref Int */

    Vector2,        /**< @ref Vector2 */
    Vector2ui,      /**< @ref Vector2ui */
    Vector2i,       /**< @ref Vector2i */

    Vector3,        /**< @ref Vector3 */
    Vector3ui,      /**< @ref Vector3ui */
    Vector3i,       /**< @ref Vector3i */

    Vector4,        /**< @ref Vector4 */
    Vector4ui,      /**< @ref Vector4ui */
    Vector4i,       /**< @ref Vector4i */

    Matrix2x2,      /**< @ref Matrix2x2 */
    Matrix2x3,      /**< @ref Matrix2x3 */
    Matrix2x4,      /**< @ref Matrix2x4 */

    Matrix3x2,      /**< @ref Matrix3x2 */
    Matrix3x3,      /**< @ref Matrix3x3 */
    Matrix3x4,      /**< @ref Matrix3x4 */

    Matrix4x2,      /**< @ref Matrix4x2 */
    Matrix4x3,      /**< @ref Matrix4x3 */

    /* Matrix4x4 not present */
};

namespace Implementation {
    template<class T> constexpr MaterialAttributeType materialAttributeTypeFor();
}

class MaterialAttributeData {
    public:
        template<class T> explicit MaterialAttributeData(MaterialAttribute name, const T& value): MaterialAttributeData{name, Implementation::materialAttributeTypeFor<T>(), sizeof(T), &value} {}

        template<class T> explicit MaterialAttributeData(Containers::StringView name, const T& value): MaterialAttributeData{name, Implementation::materialAttributeTypeFor<T>(), sizeof(T), &value} {}

        constexpr MaterialAttributeType type() const { return _type; }

    private:
        /* TODO: constexpr copy the name how? */
        /* TODO: actually how the fuck do that with predef'd names? ugh */
        /* TODO: and then we need to reinterpret the value as a char* anyway and copy byte by byte */
        constexpr explicit MaterialAttributeData(Containers::StringView name, const MaterialAttributeType type, std::size_t typeSize, const void* value);

        CORRADE_ALIGNAS(8) MaterialAttributeType _type;
        char _data[63];
};

namespace Implementation {
    #define _c(type) template<> constexpr MaterialAttributeType materialAttributeTypeFor<type>() { \
        return MaterialAttributeType::type;                                 \
    }
    _c(Float)
    _c(UnsignedInt)
    _c(Int)
    _c(Vector2)
    _c(Vector2ui)
    _c(Vector2i)
    _c(Vector3)
    _c(Vector3ui)
    _c(Vector3i)
    _c(Vector4)
    _c(Vector4ui)
    _c(Vector4i)
    _c(Matrix2x2)
    _c(Matrix2x3)
    _c(Matrix2x4)
    _c(Matrix3x2)
    _c(Matrix3x3)
    _c(Matrix3x4)
    _c(Matrix4x2)
    _c(Matrix4x3)
    #undef _c
    template<> constexpr MaterialAttributeType materialAttributeTypeFor<bool>() {
        return MaterialAttributeType::Bool;
    }

    MAGNUM_TRADE_EXPORT std::size_t materialAttributeTypeSize(MaterialAttributeType type);
}

/**
@brief Material data
@m_since_latest

*/
class MaterialData {
    public:
        explicit MaterialData(Containers::Array<MaterialAttributeData>&& data);

    private:
        Containers::Array<MaterialAttributeData> _data;
};

}}

#endif
