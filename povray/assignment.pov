#include "colors.inc" // Predifined colors.

// Camera declaration.
camera {
    location <-0.25, -1, -1> // Main camera location.
    //location <-5, 0, 5> // Helper outside camera location.
    angle 90 // Horizontal viewing angle, less than 180 degrees.
    look_at <0, 0.5, 1>
}

// Room box with the window.
difference {
    // Outer room box.
    box {
        <-1.1, -1.1, -1.1>, <1.1, 1.1, 1.1>
        pigment {
            color White
        }
    }
    // Inner room box to subtract.
    box {
        <-1, -1, -1>, <1, 1, 1>
        pigment {
            color Gray
        }
    }
    // Window box to subtract.
    box {
        <-0.3, -0.4, 0.9>, <0.3, 0.5, 1.2>
        pigment {
            color Gray
        }
    }    
}


// Helper light source.
light_source {
    <0, 0, 0>
    color White
}
