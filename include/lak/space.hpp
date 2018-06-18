/*
MIT License

Copyright (c) 2018 LAK132

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// #include <GL/gl3w.h>
#ifndef LAK_SPACE_VEC3
#include <glm/vec3.hpp>
#define LAK_SPACE_VEC3 glm::vec3
#endif

#ifndef LAK_SPACE_VEC4
#include <glm/vec4.hpp>
#define LAK_SPACE_VEC4 glm::vec4
#endif

#ifndef LAK_SPACE_MAT4
#include <glm/mat4x4.hpp>
#define LAK_SPACE_MAT4 glm::mat4
#endif

#if !defined(LAK_SPACE_TRANSLATE) || !defined(LAK_SPACE_ROTATE) || !defined(LAK_SPACE_SCALE)
#include <glm/gtc/matrix_transform.hpp>
#endif
#ifndef LAK_SPACE_TRANSLATE
#define LAK_SPACE_TRANSLATE glm::translate
#endif
#ifndef LAK_SPACE_ROTATE
#define LAK_SPACE_ROTATE glm::rotate
#endif
#ifndef LAK_SPACE_SCALE
#define LAK_SPACE_SCALE glm::scale
#endif

#include <vector>
#include <memory>

#ifndef LAK_SPACE_H
#define LAK_SPACE_H

namespace lak
{
    using std::vector;
    using std::shared_ptr;
    using std::weak_ptr;
    using std::make_shared;
    struct transform_t
    {
        const static LAK_SPACE_VEC4 XUP;
        const static LAK_SPACE_VEC4 YUP;
        const static LAK_SPACE_VEC4 ZUP;
        const static LAK_SPACE_VEC4 WUP;
        const static LAK_SPACE_MAT4 IDEN;
        LAK_SPACE_MAT4 translation = IDEN;
        LAK_SPACE_MAT4 rotation = IDEN;
        LAK_SPACE_MAT4 scale = IDEN;
        LAK_SPACE_MAT4 combined = IDEN;

        transform_t& addTranslation(const LAK_SPACE_VEC3& displace)
        {
            translation = LAK_SPACE_TRANSLATE(translation, displace);
            return *this;
        }
        transform_t& addTranslation(LAK_SPACE_VEC3&& displace)
        {
            return addTranslation(displace);
        }

        transform_t& setTranslation(const LAK_SPACE_VEC3& position)
        {
            translation = LAK_SPACE_TRANSLATE(IDEN, position);
            return *this;
        }
        transform_t& setTranslation(LAK_SPACE_VEC3&& position)
        {
            return setTranslation(position);
        }

        transform_t& addScale(const LAK_SPACE_VEC3& sca)
        {
            scale = LAK_SPACE_SCALE(scale, sca + LAK_SPACE_VEC3(1.0f, 1.0f, 1.0f));
            return *this;
        }
        transform_t& addScale(LAK_SPACE_VEC3&& sca)
        {
            return addScale(sca);
        }

        transform_t& setScale(const LAK_SPACE_VEC3& sca)
        {
            scale = LAK_SPACE_SCALE(IDEN, sca);
            return *this;
        }
        transform_t& setScale(LAK_SPACE_VEC3&& sca)
        {
            return setScale(sca);
        }

        transform_t& clear()
        {
            translation = IDEN;
            rotation = IDEN;
            scale = IDEN;
            return *this;
        }
        transform_t& make()
        {
            // combined = translation * rotation * scale;
            combined = scale * rotation * translation;
            return *this;
        }

        template <typename T>
        transform_t& addRotation(T angle, const LAK_SPACE_VEC3& axis)
        {
            rotation = LAK_SPACE_ROTATE(rotation, angle, axis);
            return *this;
        }
        template <typename T>
        transform_t& addRotation(T angle, LAK_SPACE_VEC3&& axis)
        {
            return addRotation(angle, axis);
        }
        template <typename T>
        transform_t& setRotation(T angle, const LAK_SPACE_VEC3& axis)
        {
            rotation = LAK_SPACE_ROTATE(IDEN, angle, axis);
            return *this;
        }
        template <typename T>
        transform_t& setRotation(T angle, LAK_SPACE_VEC3&& axis)
        {
            return setRotation(angle, axis);
        }
    };

    const LAK_SPACE_VEC4 transform_t::XUP = {1.0f, 0.0f, 0.0f, 0.0f};
    const LAK_SPACE_VEC4 transform_t::YUP = {0.0f, 1.0f, 0.0f, 0.0f};
    const LAK_SPACE_VEC4 transform_t::ZUP = {0.0f, 0.0f, 1.0f, 0.0f};
    const LAK_SPACE_VEC4 transform_t::WUP = {0.0f, 0.0f, 0.0f, 1.0f};
    const LAK_SPACE_MAT4 transform_t::IDEN = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    };

    struct deltaTransform_t : public transform_t
    {
        LAK_SPACE_VEC3 rotAccel = LAK_SPACE_VEC3(0.0f);
        LAK_SPACE_VEC3 rotVel = LAK_SPACE_VEC3(0.0f);
        LAK_SPACE_VEC3 rot = LAK_SPACE_VEC3(0.0f);
        LAK_SPACE_VEC3 posAccel = LAK_SPACE_VEC3(0.0f);
        LAK_SPACE_VEC3 posVel = LAK_SPACE_VEC3(0.0f);
        LAK_SPACE_VEC3 pos = LAK_SPACE_VEC3(0.0f);
        void update(float delta)
        {
            rotVel += rotAccel * delta;
            rot += rotVel * delta;
            setRotation(rot.x, XUP);
            addRotation(rot.y, YUP);
            addRotation(rot.z, ZUP);
            posVel += posAccel * delta;
            pos += posVel * delta;
            setTranslation(pos);
            make();
        }
    };

    struct referenceFrame_t
    {
        vector<referenceFrame_t> children;
        weak_ptr<referenceFrame_t> parent;
        deltaTransform_t transform;
        LAK_SPACE_MAT4 getTransform()
        {
            transform.make();
            if(auto par = parent.lock())
                return transform.combined * par->getTransform();
            else
                return transform.combined;
        }
    };
}

#endif // LAK_SPACE_H