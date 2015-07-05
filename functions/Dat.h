#pragma once

typedef enum DatField DatField;
enum DatField {
        dat_field_floor    = 0x0c,
        dat_field_blocking = 0x10
};

int32_t (*dat_get_field)(int32_t obj_id,  DatField field) = (void *)0x081e2720;
