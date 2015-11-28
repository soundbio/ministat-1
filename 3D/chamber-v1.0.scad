$fn=100;	// make round things round instead of faceted

/* helper shape for sloping support under a ledge */
module triangleBrace(L,T,H) {
	translate([0,T/2,-H/2])
	rotate([90,0,0])
	scale([L,H,T])
		polyhedron(
			points=[ [0,0,0],[0,1,0],[1,1,0],[0,0,1],[0,1,1],[1,1,1] ],
			faces=[ [0,2,1], [3,4,5], [0,1,4,3], [1,2,5,4], [0,3,5,2] ]
		);
}

/* helper shape for creating sloped roofs */
module roundBevel(h,od,theta)
{
	rotate_extrude($fn=100)
		polygon(points=[[0,0],[0,h],[od,h-od*tan(theta)/2],[od,0]]);
}

module windowTool(L,W,H,theta) {
	rotate([90,0,0])
	linear_extrude(L,center=true){
		polygon([[-W/2, 0], [-W/2, H-W*tan(theta)/2], [0,H], [W/2, H-W*tan(theta)/2], [W/2,0]], convexity = 3);
	}
}

/* 
	detector/emitter mount

	length  L end-to-end length
	width   W should be diameter of tube + 4mm (i.e. 2mm wall)
	height  H is vertical 'thickness'
	channel widthPercent is percent of mount width
	channel depthPercent is percent of mount height
	
*/
module detectorMount(L,W,H,widthPercent,depthPercent,){
	translate([0,0,H/2]){
		difference(){
			cube(size=[L,W,H], center=true);
			translate([0,0,-H/2-W*widthPercent/2+H*depthPercent])
			rotate([0,90,0])
				cylinder(h=L*.9,r=W*widthPercent/2,center=true);
			translate([0,0,-H/2-W*widthPercent+H*depthPercent])
				cube(size=[L*.9,W*widthPercent,W*widthPercent], center=true);
		}
	}
}

module chamber(L,W,H,MH,id) {
	difference(){
		union(){
			/* start with mount */
			detectorMount(L,W,MH,9/W,.8);

			/* add cylinder to mount */
			translate([0,0,H-MH-.1])
				cylinder(h=H-MH+.1,r=W/2,center=true);

		}

		/* bevel roof */
		translate([0,0,-1])
		roundBevel(H+2,id/2,43.8);

		/* remove roof core */
		cylinder(H+2,r=id/4,center=false);

		/* beam windows */
		translate([0,0,2])
		windowTool(2*W,.75*id,MH-4,atan(.4));

		/* tube windows */
		union(){
			rotate(a=[0,0,-30])
			translate([0,L,MH])
			windowTool(2*L,.75*id,H-MH-5,atan(.55));
			
			rotate(a=[0,0,210])
			translate([0,L,MH])
			windowTool(2*L,.75*id,H-MH-5,atan(.55));
	
			rotate(a=[0,0,90])
			translate([0,L,MH])
			windowTool(2*L,.75*id,H-MH-5,atan(.55));
		}

	}
}


len=25+4+22+22;
wid=25+4;
ht=12+22;
mht=12;
indiam=25;

chamber(len, wid, ht, mht, indiam);
