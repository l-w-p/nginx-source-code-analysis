
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct ngx_list_part_s  ngx_list_part_t;

// 链表结点
struct ngx_list_part_s {
    void             *elts;  // 当前链表结点中元素数组的起始地址
    ngx_uint_t        nelts; // 当前链表结点中已使用的基本存储单元的个数
    ngx_list_part_t  *next;  // 下一个链表结点
};


// 链表管理结构
typedef struct {
    ngx_list_part_t  *last;   // 当前链表中最后一个链表结点
    ngx_list_part_t   part;   // 当前链表中第一个链表结点
    size_t            size;   // 当前链表中每个链表结点中基本存储单元的大小
    ngx_uint_t        nalloc; // 当前链表中每个链表结点中基本存储单元的个数
    ngx_pool_t       *pool;   // 当前链表用于分配内存的内存池指针
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);

/* brief  : 初始化链表, 为当前链表分配一个链表结点,
 *          并设置每个链表结点提供 n 个 size 大小的基本存储单元
 * param  : [in] list 链表管理结构体的指针
 * param  : [in] pool 当前链表用于分配内存的内存池指针
 * param  : [in] n 当前链表中每个链表结点中基本存储单元的个数
 * param  : [in] size 当前链表中每个链表结点中基本存储单元的大小
 * return : NGX_ERROR/NGX_OK
 */
static ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    list->part.elts = ngx_palloc(pool, n * size);
    if (list->part.elts == NULL) {
        return NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.next = NULL;
    list->last = &list->part;
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    return NGX_OK;
}


/*
 * brief  : 遍历链表 list 中的基本存储单元, 获取其中数据
 * note   : type 需要根据实际情况采用相对应的数据结构
 */

/*
 *
 *  the iteration through the list:
 *
 *  part = &list.part;
 *  data = part->elts;
 *
 *  for (i = 0 ;; i++) {
 *
 *      // 超过当前结点基本存储单元的个数
 *      if (i >= part->nelts) {
 *          // 当前结点为链表的最后一个结点
 *          if (part->next == NULL) {
 *              break;
 *          }
 *
 *          // 跳到下一个结点
 *          part = part->next;
 *          // data 指向下一个结点的第一个基本存储单元
 *          data = part->elts;
 *          i = 0;
 *      }
 *
 *      // 处理当前存储单元中的数据
 *      ...  data[i] ...
 *
 *  }
 */


void *ngx_list_push(ngx_list_t *list);


#endif /* _NGX_LIST_H_INCLUDED_ */
