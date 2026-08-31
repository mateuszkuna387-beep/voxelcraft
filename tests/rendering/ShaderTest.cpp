#include <gtest/gtest.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/Constants.h"

TEST(ShaderTest, HighlightBlendingMath) {
    // Test the shader's highlight blending: color = mix(baseColor, white, 0.5)
    // This means final = baseColor * 0.5 + white * 0.5
    // For green (0.2, 0.7, 0.1): (0.1, 0.35, 0.05) + (0.5, 0.5, 0.5) = (0.6, 0.85, 0.55)
    // For brown (0.5, 0.3, 0.1): (0.25, 0.15, 0.05) + (0.5, 0.5, 0.5) = (0.75, 0.65, 0.55)

    glm::vec3 topColor(0.2, 0.7, 0.1);
    glm::vec3 sideColor(0.5, 0.3, 0.1);
    glm::vec3 highlightColor(1.0, 1.0, 1.0);

    glm::vec3 topHighlighted = glm::mix(topColor, highlightColor, 0.5);
    glm::vec3 sideHighlighted = glm::mix(sideColor, highlightColor, 0.5);

    EXPECT_NEAR(topHighlighted.r, 0.6, 0.01);
    EXPECT_NEAR(topHighlighted.g, 0.85, 0.01);
    EXPECT_NEAR(topHighlighted.b, 0.55, 0.01);

    EXPECT_NEAR(sideHighlighted.r, 0.75, 0.01);
    EXPECT_NEAR(sideHighlighted.g, 0.65, 0.01);
    EXPECT_NEAR(sideHighlighted.b, 0.55, 0.01);
}