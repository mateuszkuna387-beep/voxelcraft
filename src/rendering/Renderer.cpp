#include "Renderer.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Camera.h"

void Renderer::clear() {
    glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::beginFrame(Camera* camera) {
    (void)camera;
}
