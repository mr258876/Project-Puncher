#ifndef _PUNCHERSTORAGETYPES_H_
#define _PUNCHERSTORAGETYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum puncher_storage_type_t
{
    PUNCHER_STORAGE_TYPE_UINT8,
    PUNCHER_STORAGE_TYPE_INT8,
    PUNCHER_STORAGE_TYPE_UINT16,
    PUNCHER_STORAGE_TYPE_INT16,
    PUNCHER_STORAGE_TYPE_UINT32,
    PUNCHER_STORAGE_TYPE_INT32,
    PUNCHER_STORAGE_TYPE_UINT64,
    PUNCHER_STORAGE_TYPE_INT64,
    PUNCHER_STORAGE_TYPE_STRING,
    PUNCHER_STORAGE_TYPE_BLOB,
    PUNCHER_STORAGE_FLOAT,
    PUNCHER_STORAGE_TYPE_DOUBLE,
    PUNCHER_STORAGE_TYPE_BOOL,
} puncher_storage_type_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif

# endif // _PUNCHERSTORAGETYPES_H_