#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ruby.h>
#include "saisoku.h"


// initialize node
node initialize_node(char moji)
{
    node work = (node)malloc(sizeof(struct _node));
    if (!work) {
        rb_raise(rb_eStandardError, "memory is not enough");
    }

    work->moji = moji;
    work->replacement = NULL;
    work->child_head = NULL;
    work->next = NULL;

    return work;
}

// add child node
void add_child(node parent, node child)
{
    if (parent->child_head) {
        child->next = parent->child_head;
    }
    parent->child_head = child;
}

// search node by use character
node search_child(node n, char moji)
{
    node child;
    
    child = n->child_head;
    while(child) {
        if (child->moji == moji) {
            break;
        }
        child = child->next;
    }

    return child;
}

// search node by use character.
// if nothing, create new node
node search_child_or_create(node n, char moji)
{
    node child;
    
    child = search_child(n, moji);
    if(!child) {
        child = initialize_node(moji);
        add_child(n, child);
    }

    return child;
}

// free memory all child and self
void destroy_node(node n)
{
    node now, next;

    now = n->child_head;
    while(now) {
        next = now->next;
        destroy_node(now);
        now = next;
    }

    if(n->replacement) {
      free(n->replacement);
    }
    free(n);
}


//-----------------------------------------------------------
// Ruby Methods
// ----------------------------------------------------------

/**
 * new
 **/
static VALUE t_new(int argc, VALUE *argv, VALUE klass)
{
    node root;
    VALUE obj;

    root = initialize_node(NULL_CHAR);

    obj = Data_Make_Struct(klass, struct _node, NULL, destroy_node, root);

    if (argc == 1) {
      rb_funcall(obj, rb_intern("add"), 1, argv[0]);
    }
    
    return obj;
}

/**
 * set keyword and replacement
 **/
static VALUE t_set_match_and_replacement(VALUE self, VALUE match, VALUE replace_str)
{
    node root, now;
    char *keyword, *work, *replacement;
    size_t i, len;

    keyword = StringValuePtr(match);
    work = StringValuePtr(replace_str);
    replacement  = (char*)malloc(strlen(work+1));
    if (!replacement) {
        rb_raise(rb_eStandardError, "memory is not enough");
    }
    strcpy(replacement, work);

    len = strlen(keyword);
    while(keyword[len - 1] == CR || keyword[len - 1] == LF ||
            keyword[len - 1] == TAB ||  keyword[len - 1] == SPACE) {
        len--;
    }

    if (len < 1) {
        return Qfalse;
    }

    Data_Get_Struct(self, struct _node, root);

    now = root;
    for(i = 0; i < len; i++) {
        now = search_child_or_create(now, keyword[i]);
    }

    now->replacement = replacement;

    return Qtrue;
}

/**
 * replace
 **/
static VALUE t_replace(VALUE self, VALUE str)
{
    node root, now, ret;
    char *text;
    char *replacement;
    int i, head_i, tail_i, copy_head_i, total_len;
    VALUE change_str;
    
    change_str = rb_str_new2(EMPTY_STRING);
    text = StringValuePtr(str);

    Data_Get_Struct(self, struct _node, root);

    now = root;
    total_len = strlen(text);
    head_i = -1;
    tail_i = -1;
    copy_head_i = 0;
    replacement = NULL;

    for(i = 0; i <= total_len; i++) {
        ret = search_child(now, text[i]);

        if (ret && i != total_len) {
            if (head_i == -1) {
                head_i = i;
            }

            if (ret->replacement) {
                tail_i = i;
                replacement = ret->replacement;
            }
            now = ret;
        } else {
            if (head_i != -1) {
                if (tail_i != -1) {
                    if (copy_head_i < head_i) {
                        rb_funcall(change_str, rb_intern("concat"), 1, rb_str_new(&text[copy_head_i], (head_i - copy_head_i)));
                    }
                    rb_funcall(change_str, rb_intern("concat"), 1, rb_str_new(replacement, strlen(replacement)));
                    i = tail_i;
                    copy_head_i = tail_i + 1;
                    tail_i = -1;
                    replacement = NULL;
                } else {
                    i = head_i;
                }
                head_i = -1;
            }
            now = root;
        }
    }

    if (copy_head_i == 0) {
        return str;
    } else {
        rb_funcall(change_str, rb_intern("concat"), 1, rb_str_new(&text[copy_head_i], (total_len - copy_head_i)));
        return change_str;
    }
}

/**
 * define class
 **/
void Init_saisoku() {
    VALUE cSaisoku;

    cSaisoku = rb_define_class("Saisoku", rb_cObject);
    rb_define_singleton_method(cSaisoku, "new", t_new, -1);
    rb_define_method(cSaisoku, "set_match_and_replacement", t_set_match_and_replacement, 2);
    rb_define_method(cSaisoku, "replace", t_replace, 1);
}
