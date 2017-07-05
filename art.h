typedef struct {
	double	v[3];
} Point, Vector, Color;

typedef struct {
	Color col;
	double Ka, Kd, Ks, Kr, Kt, n, index;
	int texture;
} Material;

extern char *art_Start(void);
extern char *art_End(void);
extern char *art_Scale(double, double, double);
extern char *art_Rotate(char, double);
extern char *art_Translate(double, double, double);
extern char *art_Shear(char, char, double);
extern char *art_InitTM(void);
extern char *art_PushTM(void);
extern char *art_PopTM(void);
extern char *art_Camera(Point, Point, Vector);
extern char *art_Perspective(double, double, double);
extern char *art_Trace(int, int, int, char *);
extern char *art_Lens(double, double);
extern char *art_Light(double, double, double, double, double);
extern char *art_Material(Material);
extern char *art_Background(Color);

extern char *art_Sphere(void);
extern char *art_Cube(void);
extern char *art_Plane(void);
extern char *art_Cylinder(void);
