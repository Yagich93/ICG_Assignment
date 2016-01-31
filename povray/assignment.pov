#include "colors.inc" // Predifined colors.

// Camera declaration.
camera {
    location <-0.25, -0.5, -1.9> // Main camera location.
    //location <-5, 0, 5> // Helper outside camera location.
    angle 90 // Horizontal viewing angle, less than 180 degrees.
    look_at <0, 0, 1>
}

// Room box with the window.
difference {
    #local clr = Gray05;
    // Outer room box.
    box {
        <-1.1, -1.1, -2.1>, <1.1, 1.1, 1.1>
        pigment {
            color White
        }
    }
    // Inner room box to subtract.
    box {
        <-1, -1, -2>, <1, 1, 1>
        pigment {
            color clr
        }
    }
    // Window box to subtract.
    box {
        <-0.3, -0.4, 0.9>, <0.3, 0.5, 1.2>
        pigment {
            color clr
        }
    }    
}

// Moonlight.
light_source {
    <-60, 80, 100>
    color rgb<0.5, 0.5, 0.8>
    parallel
    point_at<0, 0, 0>
}

// Helper light source.
light_source {
    <0.9, 0, 0>
    color Gray10
    fade_distance 0.5
    fade_power 0.1
}