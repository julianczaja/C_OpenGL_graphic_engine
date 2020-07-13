#ifndef _MATHOPENGL_H_
#define _MATHOPENGL_H_

// TODO:
// - comparison
// - invert
// - generate projection matrix
// - generate look at matrix

typedef float vec3f[3];
typedef float vec4f[4];
typedef float mat3f[3 * 3];
typedef float mat4f[4 * 4];

void vec3fAdd(vec3f this, vec3f v);
void vec3fSubtract(vec3f this, vec3f v);
void vec3fMultiply(vec3f this, vec3f v); // dot product

void vec4fAdd(vec4f this, vec4f v);
void vec4fSubtract(vec4f this, vec4f v);
void vec4fMultiply(vec4f this, vec4f v); // dot product

void mat3fCopy(mat3f this, mat3f m); // from "this" to "v"
void mat3fAdd(mat3f this, mat3f m);
void mat3fSubtract(mat3f this, mat3f m);
void mat3fMultiply(mat3f this, mat3f m);

void mat4fCopy(mat4f this, mat4f m); // from "this" to "v"
void mat4fIdentity(mat4f this);
void mat4fAdd(mat4f this, mat4f m);
void mat4fSubtract(mat4f this, mat4f m);
void mat4fMultiply(mat4f this, mat4f m);
void mat4fMultiplyVec4f(vec4f dest, mat4f this, vec4f v);

// returns translation matrix 4x4
void mat4fTranslate(mat4f dest, float x, float y, float z); 

// returns scale matrix 4x4
void mat4fScale(mat4f dest, float x, float y, float z); 

// returns rotation matrix 4x4
void mat4Rotate(mat4f dest, float x, float y, float z, float theta); 


#endif