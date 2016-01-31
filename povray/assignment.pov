#include "colors.inc" // Predifined colors.

// Camera declaration.
camera {
    location <0, 0, 0>
    angle 50 // Horizontal viewing angle, less than 180 degrees.
    look_at <0, 1, 1>
}

// Room box.
box {
    <-1, -1, -1>, <1, 1, 1>
    pigment {
        color Gray
    }
}

// Helper light source.
light_source {
    <0, 0, 0>
    color White
}
