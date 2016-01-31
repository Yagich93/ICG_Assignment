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

// Flashlight.
union {
    // Flashlight cylinder.
    cylinder {
        <0, -0.075, 0>, <0, 0.075, 0>, 0.02
    }
    // Flashlight cone.
    difference {
        cone {
            <0, 0.1, 0>, 0.03, <0, 0.075, 0>, 0.02
        }
        cone {
            <0, 0.11, 0>, 0.028, <0, 0.074, 0>, 0.018
            pigment {
                color Black
            }
        }
    }
    // Light.
    light_source {
        <0, 0.075, 0>
        color rgb<0.5, 0.5, 0.3>
        spotlight
        radius 20
        falloff 30
        tightness 10
        point_at<0, 0.1, 0>
        looks_like {
            cone {
                <0, 0.025, 0>, 0.028, <0, 0, 0>, 0.018
            }
        }
    }
    pigment {
        color White
    }
    rotate<90, 0, 0>
    rotate<0, 30, 0>
    translate<-0.75, -1+0.028, -0.3>
}

// Moonlight.
light_source {
    <-60, 80, 100>
    color rgb<0.4, 0.4, 0.7>
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