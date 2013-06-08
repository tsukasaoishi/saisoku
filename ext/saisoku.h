#define CR '\r'
#define LF '\n'
#define TAB '\t'
#define SPACE ' '
#define NULL_CHAR '\0'
#define EMPTY_STRING ""

// node is 1 byte character
typedef struct _node {
    char    moji;               // character
    char    *replacement;       // if NULL, not terminal. else replace string

    struct _node    *child_head;// head of child list
    struct _node    *next;      // pointer of sibling node
} *node;


// initialize node
node initialize_node(char);

// add child node
void add_child(node, node);

// search node by use character
node search_child(node, char);

// search node by use character.
// if nothing, create new node
node search_child_or_create(node, char);

// free memory all child and self
void destroy_node(node);

//-----------------------------------------------------------
// Ruby Methods
// ----------------------------------------------------------

// new
static VALUE t_new(int, VALUE *, VALUE);

// add
static VALUE t_add(VALUE, VALUE);

// replace
static VALUE t_replace(VALUE, VALUE);

// defined class
void Init_saisoku();

