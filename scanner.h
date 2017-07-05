/* commands */
#define ERROR		0
#define QUIT		1
#define INCLUDE		2
#define COMMENT		3
#define SCALE		4
#define ROTATE		5
#define TRANSLATE	6
#define SHEAR		7
#define INIT_TM		8
#define PUSH_TM		9
#define POP_TM		10
#define CAMERA		11
#define PERSPECTIVE	12
#define TRACE		13
#define BACKGROUND	14
#define LIGHT		15
#define MATERIAL	16
#define LENS		17

#define SPHERE		18
#define PLANE		19
#define CUBE		20
#define CYLINDER	21

typedef struct {
	char *word;
	int code;
} Words;

static Words Commands[]= {
	"quit", QUIT,
	"include", INCLUDE,
	"//", COMMENT,
	"scale", SCALE,
	"rotate", ROTATE,
	"translate", TRANSLATE,
	"shear", SHEAR,
	"initTM", INIT_TM,
	"pushTM", PUSH_TM,
	"popTM", POP_TM,
	"camera", CAMERA,
	"perspective", PERSPECTIVE,
	"trace", TRACE,
	"background", BACKGROUND,
	"light", LIGHT,
	"material", MATERIAL,
	"lens", LENS,
	"sphere", SPHERE,
	"plane", PLANE,
	"cube", CUBE,
	"cylinder", CYLINDER,
	NULL, ERROR
};
