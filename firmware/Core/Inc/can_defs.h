/**
 * @file
 * @brief Provides definitions of CAN message formats
*/
#ifndef CAN_DEFS_H
#define CAN_DEFS_H

#include <assert.h>
#include <stdint.h>

#include "gps.h"
#include "utils.h"

/**
 * @brief Check CAN union members occupy same space
 * @param union Typedef name of union to check
 * @return None
 * @note If using union to reinterpret same data, both data and bytes fields must be identical in size
 *       Also checks data is less than 8 bytes limit for CAN
*/
#define CHECK_UNION_SIZE(union) static_assert(sizeof(NULL_TYPE(union).data) == sizeof(NULL_TYPE(union).bytes)); \
                                static_assert(sizeof(NULL_TYPE(union).data) <= 8);


#define CAN_ID_GPS_BASE 0x750


#define CAN_ID_GPS_TIME (CAN_ID_GPS_BASE)
typedef union {
    time_t data;
    uint8_t bytes[8];
} __attribute__ ((packed)) CAN_GPS_Time_t;
CHECK_UNION_SIZE(CAN_GPS_Time_t)


#define CAN_ID_GPS_COORD (CAN_ID_GPS_BASE + 1)
typedef union {
    struct {float lattitude; float longitude;} data;
    uint8_t bytes[8];
} __attribute__ ((packed)) CAN_GPS_Coord_t;
CHECK_UNION_SIZE(CAN_GPS_Coord_t)


#define CAN_ID_GPS_ALTVEL (CAN_ID_GPS_BASE + 2)
typedef union {
    struct {float altitude; float speed;} data;
    uint8_t bytes[8];
} __attribute__ ((packed)) CAN_GPS_AltVel_t;
CHECK_UNION_SIZE(CAN_GPS_AltVel_t)


#endif /* CAN_DEFS_H */