
//LippedReadWheel.scad (c) by Embecosm Ltd.
//
//LippedReadWheel.scad is licensed under a
//Creative Commons Attribution-ShareAlike 4.0 International License.
//
//You should have received a copy of the license along with this
//work. If not, see <http://creativecommons.org/licenses/by-sa/4.0/>.

difference(){
    union(){
        translate([0,0,-3.7])
            cylinder(r=9.3, h=1, $fn=16, center=true);
        translate([0,0,3.7])
            cylinder(r=9.3, h=1, $fn=16, center=true);
        cylinder(r=8.3, h=8.4, $fn=16, center=true);
    };
    difference(){
        cylinder(r=1.85, h=14, $fn=60, center=true);
        translate([0,2.8,0])
            cube([3,3,16], center=true);
    };
    translate([0,5,0])
        rotate([90,0,0])
            cylinder(r=1.8, h=10, $fn=60, center=true);
    translate([0,4,0])
        cube([6.2,3,14], center=true);
};
