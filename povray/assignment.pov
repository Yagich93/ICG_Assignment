#include "colors.inc" // Predifined colors.

// Global ambient light.
global_settings { ambient_light Gray05 }

// Camera declaration.
camera {
    location <-0.25, -0.3, -1.9> // Main camera location.
    //location <-5, 0, 5> // Helper outside camera location.
    angle 90 // Horizontal viewing angle, less than 180 degrees.
    look_at <0, 0, 1>
}

// Room box with the window.
difference {
    #local clr = Gray50;
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

// Table.
union {
    // Table cover.
    box {
        <-0.5, -0.02, -0.3>, <0.5, 0.02, 0.3>
        translate<0, 0.25, 0>
    }
    // Table leg template.
    #local table_leg = box {
        <-0.02, -0.25, -0.02>, <0.02, 0.25, 0.02>
    }
    // 4 legs.
    object {
        table_leg
        translate<-0.48, 0, -0.28>
    }
    object {
        table_leg
        translate<0.48, 0, -0.28>
    }
    object {
        table_leg
        translate<-0.48, 0, 0.28>
    }
    object {
        table_leg
        translate<0.48, 0, 0.28>
    }
    // Common texture of the table.
    pigment {
        wood
        frequency 10
        turbulence 0.1
    }
    // Moving table into position.
    translate<0.5, -0.75, 0.7>
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
    #local clr = 0.01;
    color rgb<clr, clr, clr>
    fade_distance 0.3
    fade_power 0.01
}