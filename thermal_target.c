/* Copyright (c) 2018, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of The Linux Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define LOG_TAG "ThermalHAL-TARGET"
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/thermal.h>
#include <stdlib.h>
#include "thermal_common.h"

#define SYSFS_PLATFORMID   "/sys/devices/soc0/soc_id"
#define SOC_INFO_NAME_LEN  15

struct therm_msm_soc_type {
    enum therm_msm_id msm_id;
    int  soc_id;
};

static struct therm_msm_soc_type msm_soc_table[] = {
    {THERM_MSM_8953, 293},
    {THERM_MSM_8953, 304},
    {THERM_MSM_8953, 338},
    {THERM_MSM_8953, 351},
    {THERM_SDM_660, 317},
    {THERM_SDM_660, 324},
    {THERM_SDM_660, 325},
    {THERM_SDM_660, 326},
    {THERM_SDM_660, 345},
    {THERM_SDM_660, 346},
    {THERM_SDM_630, 318},
    {THERM_SDM_630, 327},
    {THERM_SDM_630, 385},
    {THERM_SDM_710, 336},
    {THERM_SDM_710, 337},
    {THERM_QCS_605, 347},
    {THERM_SDM_632, 349},
    {THERM_SDM_632, 350},
    {THERM_SDM_439, 353},
    {THERM_SDM_439, 354},
    {THERM_SDM_439, 363},
    {THERM_SDM_439, 364},
    {THERM_MSMNILE, 339},
    {THERM_MSMNILE, 362},
    {THERM_MSMNILE, 367},
};

static char *cpu_sensors_msmnile[] =
{
    "cpu-0-0-usr",
    "cpu-0-1-usr",
    "cpu-0-2-usr",
    "cpu-0-3-usr",
    "cpu-1-0-usr",
    "cpu-1-1-usr",
    "cpu-1-2-usr",
    "cpu-1-3-usr",
};

static char *misc_sensors_msmnile[] =
{
    "gpuss-0-usr",
    "battery",
    "xo-therm"
};

static struct target_therm_cfg sensor_cfg_msmnile[] = {
    {
        .type = DEVICE_TEMPERATURE_CPU,
        .sensor_list = cpu_sensors_msmnile,
        .sens_cnt = ARRAY_SIZE(cpu_sensors_msmnile),
        .mult = 0.001,
    },
    {
        .type = DEVICE_TEMPERATURE_GPU,
        .sensor_list = &misc_sensors_msmnile[0],
        .sens_cnt = 1,
        .mult = 0.001,
        .label = "GPU",
    },
    {
        .type = DEVICE_TEMPERATURE_BATTERY,
        .sensor_list = &misc_sensors_msmnile[1],
        .sens_cnt = 1,
        .mult = 0.001,
        .label = "battery",
    },
    {
        .type = DEVICE_TEMPERATURE_SKIN,
        .sensor_list = &misc_sensors_msmnile[2],
        .sens_cnt = 1,
        .mult = 0.001,
        .label = "skin",
    }
};

static char *cpu_sensors_660[] =
{
    "tsens_tz_sensor1",
    "tsens_tz_sensor1",
    "tsens_tz_sensor1",
    "tsens_tz_sensor1",
    "tsens_tz_sensor3",
    "tsens_tz_sensor4",
    "tsens_tz_sensor5",
    "tsens_tz_sensor6",
};

static char *misc_sensors_660[] =
{
    "tsens_tz_sensor8",
    "battery",
    "xo_therm"
};

static struct target_therm_cfg sensor_cfg_660[] = {
    {
        .type = DEVICE_TEMPERATURE_CPU,
        .sensor_list = cpu_sensors_660,
        .sens_cnt = ARRAY_SIZE(cpu_sensors_660),
        .mult = 0.1,
    },
    {
        .type = DEVICE_TEMPERATURE_GPU,
        .sensor_list = &misc_sensors_660[0],
        .sens_cnt = 1,
        .mult = 0.1,
        .label = "GPU",
    },
    {
        .type = DEVICE_TEMPERATURE_BATTERY,
        .sensor_list = &misc_sensors_660[1],
        .sens_cnt = 1,
        .mult = 0.001,
        .label = "battery",
    },
    {
        .type = DEVICE_TEMPERATURE_SKIN,
        .sensor_list = &misc_sensors_660[2],
        .sens_cnt = 1,
        .mult = 1,
        .label = "skin",
    }
};

static char *cpu_sensors_630[] =
{
    "tsens_tz_sensor3",
    "tsens_tz_sensor4",
    "tsens_tz_sensor5",
    "tsens_tz_sensor6",
    "tsens_tz_sensor7",
    "tsens_tz_sensor7",
    "tsens_tz_sensor7",
    "tsens_tz_sensor7",
};

static struct target_therm_cfg sensor_cfg_630[] = {
    {
        .type = DEVICE_TEMPERATURE_CPU,
        .sensor_list = cpu_sensors_630,
        .sens_cnt = ARRAY_SIZE(cpu_sensors_630),
        .mult = 0.1,
    },
    {
        .type = DEVICE_TEMPERATURE_GPU,
        .sensor_list = &misc_sensors_660[0],
        .sens_cnt = 1,
        .mult = 0.1,
        .label = "GPU",
    },
    {
        .type = DEVICE_TEMPERATURE_BATTERY,
        .sensor_list = &misc_sensors_660[1],
        .sens_cnt = 1,
        .mult = 0.001,
        .label = "battery",
    },
    {
        .type = DEVICE_TEMPERATURE_SKIN,
        .sensor_list = &misc_sensors_660[2],
        .sens_cnt = 1,
        .mult = 1,
        .label = "skin",
    }
};

static char *cpu_sensors_8953[] =
{
    "tsens_tz_sensor9",
    "tsens_tz_sensor10",
    "tsens_tz_sensor11",
    "tsens_tz_sensor12",
    "tsens_tz_sensor4",
    "tsens_tz_sensor5",
    "tsens_tz_sensor6",
    "tsens_tz_sensor7",
};

static char *misc_sensors_8953[] =
{
    "tsens_tz_sensor15",
    "battery",
    "xo_therm"
};

static struct target_therm_cfg sensor_cfg_8953[] = {
    {
        .type = DEVICE_TEMPERATURE_CPU,
        .sensor_list = cpu_sensors_8953,
        .sens_cnt = ARRAY_SIZE(cpu_sensors_8953),
        .mult = 0.1,
    },
    {
        .type = DEVICE_TEMPERATURE_GPU,
        .sensor_list = &misc_sensors_8953[0],
        .sens_cnt = 1,
        .mult = 0.1,
        .label = "GPU",
    },
    {
        .type = DEVICE_TEMPERATURE_BATTERY,
        .sensor_list = &misc_sensors_8953[1],
        .sens_cnt = 1,
        .mult = 0.001,
        .label = "battery",
    },
    {
        .type = DEVICE_TEMPERATURE_SKIN,
        .sensor_list = &misc_sensors_8953[2],
        .sens_cnt = 1,
        .mult = 1,
        .label = "skin",
    }
};


static char *cpu_sensors_710[] =
{
    "cpu0-silver-usr",
    "cpu1-silver-usr",
    "cpu2-silver-usr",
    "cpu3-silver-usr",
    "cpu4-silver-usr",
    "cpu5-silver-usr",
    "cpu0-gold-usr",
    "cpu1-gold-usr",
};

static char *misc_sensors_710[] =
{
    "gpu0-usr",
    "battery",
    "xo-therm-adc"
};

static struct target_therm_cfg sensor_cfg_710[] = {
    {
        .type = DEVICE_TEMPERATURE_CPU,
        .sensor_list = cpu_sensors_710,
        .sens_cnt = ARRAY_SIZE(cpu_sensors_710),
        .mult = 0.001,
    },
    {
        .type = DEVICE_TEMPERATURE_GPU,
        .sensor_list = &misc_sensors_710[0],
        .sens_cnt = 1,
        .mult = 0.001,
        .label = "GPU",
    },
    {
        .type = DEVICE_TEMPERATURE_BATTERY,
        .sensor_list = &misc_sensors_710[1],
        .sens_cnt = 1,
        .mult = 0.001,
        .label = "battery",
    },
    {
        .type = DEVICE_TEMPERATURE_SKIN,
        .sensor_list = &misc_sensors_710[2],
        .sens_cnt = 1,
        .mult = 0.001,
        .label = "skin",
    }
};

static char *cpu_sensors_632[] =
{
    "cpuss0-usr",
    "cpuss0-usr",
    "cpuss0-usr",
    "cpuss0-usr",
    "apc1-cpu0-usr",
    "apc1-cpu1-usr",
    "apc1-cpu2-usr",
    "apc1-cpu3-usr",
};

static char *misc_sensors_632[] =
{
    "gpu0-usr",
    "battery",
    "quiet-therm-adc"
};

static struct target_therm_cfg sensor_cfg_632[] = {
    {
        .type = DEVICE_TEMPERATURE_CPU,
        .sensor_list = cpu_sensors_632,
        .sens_cnt = ARRAY_SIZE(cpu_sensors_632),
        .mult = 0.001,
    },
    {
        .type = DEVICE_TEMPERATURE_GPU,
        .sensor_list = &misc_sensors_632[0],
        .sens_cnt = 1,
        .mult = 0.001,
        .label = "GPU",
    },
    {
        .type = DEVICE_TEMPERATURE_BATTERY,
        .sensor_list = &misc_sensors_632[1],
        .sens_cnt = 1,
        .mult = 0.001,
        .label = "battery",
    },
    {
        .type = DEVICE_TEMPERATURE_SKIN,
        .sensor_list = &misc_sensors_632[2],
        .sens_cnt = 1,
        .mult = 0.001,
        .label = "skin",
    }
};

static char *cpu_sensors_439[] =
{
    "apc1-cpu0-usr",
    "apc1-cpu1-usr",
    "apc1-cpu2-usr",
    "apc1-cpu3-usr",
    "cpuss0-usr",
    "cpuss0-usr",
    "cpuss0-usr",
    "cpuss0-usr",
};

static char *misc_sensors_439[] =
{
    "gpu-usr",
    "battery",
    "quiet-therm-adc"
};

static struct target_therm_cfg sensor_cfg_439[] = {
    {
        .type = DEVICE_TEMPERATURE_CPU,
        .sensor_list = cpu_sensors_439,
        .sens_cnt = ARRAY_SIZE(cpu_sensors_439),
        .mult = 0.001,
    },
    {
        .type = DEVICE_TEMPERATURE_GPU,
        .sensor_list = &misc_sensors_439[0],
        .sens_cnt = 1,
        .mult = 0.001,
        .label = "GPU",
    },
    {
        .type = DEVICE_TEMPERATURE_BATTERY,
        .sensor_list = &misc_sensors_439[1],
        .sens_cnt = 1,
        .mult = 0.001,
        .label = "battery",
    },
    {
        .type = DEVICE_TEMPERATURE_SKIN,
        .sensor_list = &misc_sensors_439[2],
        .sens_cnt = 1,
        .mult = 0.001,
        .label = "skin",
    }
};

static int get_soc_info(char *buf)
{
    int ret = 0;

    ret = read_line_from_file(SYSFS_PLATFORMID, buf, SOC_INFO_NAME_LEN);
    if (ret < 0) {
        ALOGE("Error getting platform_id %d", ret);
        return ret;
    }
    if (ret && buf[ret - 1] == '\n')
        buf[ret - 1] = '\0';

    return ret;
}

enum therm_msm_id therm_get_soc_id(void)
{
    static enum therm_msm_id msm_id;
    int soc_id = -1;

    if (!msm_id) {
        int idx;
        char buf[SOC_INFO_NAME_LEN];

        if (get_soc_info(buf) <= 0)
            return msm_id;

        soc_id = atoi(buf);
        for (idx = 0; idx < ARRAY_SIZE(msm_soc_table); idx++) {
            if (soc_id == msm_soc_table[idx].soc_id) {
                msm_id = msm_soc_table[idx].msm_id;
                break;
            }
        }
        if (!msm_id)
            ALOGE("Unknown target identified with soc id %d\n", soc_id);
   }

   return msm_id;
}

ssize_t get_temperatures(thermal_module_t *module, temperature_t *list, size_t size) {
    ALOGD("Entering %s",__func__);
    static int thermal_sens_size;

    if (!thermal_sens_size) {
	struct target_therm_cfg *cfg = NULL;
        size_t num_cfg = 0;

        switch (therm_get_soc_id()) {
            case THERM_MSM_8953:
                cfg = sensor_cfg_8953;
                num_cfg = ARRAY_SIZE(sensor_cfg_8953);
                break;
            case THERM_SDM_660:
                cfg = sensor_cfg_660;
                num_cfg = ARRAY_SIZE(sensor_cfg_660);
                break;
            case THERM_SDM_630:
                cfg = sensor_cfg_630;
                num_cfg = ARRAY_SIZE(sensor_cfg_630);
                break;
            case THERM_SDM_710:
            case THERM_QCS_605:
                cfg = sensor_cfg_710;
                num_cfg = ARRAY_SIZE(sensor_cfg_710);
                break;
            case THERM_SDM_632:
                cfg = sensor_cfg_632;
                num_cfg = ARRAY_SIZE(sensor_cfg_632);
                break;
            case THERM_SDM_439:
                cfg = sensor_cfg_439;
                num_cfg = ARRAY_SIZE(sensor_cfg_439);
                break;
            case THERM_MSMNILE:
                cfg = sensor_cfg_msmnile;
                num_cfg = ARRAY_SIZE(sensor_cfg_msmnile);
                break;
            default:
                cfg = NULL;
                num_cfg = 0;
                break;
	}
	if (cfg == NULL || num_cfg == 0) {
            ALOGE("No target specific sensor config\n");
            return 0;
	}
	thermal_sens_size = thermal_zone_init(cfg, num_cfg);
	if (thermal_sens_size <= 0) {
            ALOGE("thermal sensor initialization is failed\n");
            thermal_sens_size = 0;
	    return 0;
        }
    }

    if (list == NULL)
        return thermal_sens_size;

    return get_temperature_for_all(list, size);
}
