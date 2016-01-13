/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Teapot.inl
// Derived from WebGL sample
// https://github.com/KhronosGroup/WebGL/blob/master/sdk/demos/google/shiny-teapot/teapot-streams.js
//
#ifndef _TEAPOT_DATA_H_
#define _TEAPOT_DATA_H_

namespace TeapotData
{

extern uint16_t numVertices;

extern float positions[];

extern float normals[];

extern float tangents[];

extern float  binormals[];

extern float  texCoords[];

extern uint16_t  indices[];

extern uint16_t numIndices;
}

namespace PlaneData
{

extern uint16_t numVertices;

extern float positions[];

extern float normals[];

extern float   texCoords[];

extern uint16_t   indices[];

extern uint16_t  numIndices;
}

#endif


