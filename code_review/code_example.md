# Good Code Bad Code?

## table of content

- [Good Code Bad Code?](#good-code-bad-code)
  - [table of content](#table-of-content)
  - [Example 1](#example-1)
  - [Example 2](#example-2)
  - [Example 3](#example-3)
  - [Example 4  (naming)](#example-4--naming)
  - [Example 5](#example-5)
  - [Example 6](#example-6)
  - [Example 7](#example-7)
  - [Example 8](#example-8)
  - [Example 9](#example-9)
  - [Example 10](#example-10)
  - [Example 11](#example-11)
  - [Example 12](#example-12)

## Example 1

- **before**

```c
void SearchValidPtrForEventLog(void)
{
    bool_t       find_it  = B_FALSE;
    uint64_t     idx      = 0;
    flash_size_t ptr_page = CONFIG_EVENT_LOG_FIRST_PAGE_ADDR;
    flash_size_t ptr_section;

    for (uint16_t i = 0; i < CONFIG_PAGE_NO; i++)
    {
        if (i > 0)
        {
            ptr_page += CONFIG_PAGE_SIZE;
        }

        flash_Read(ptr_page, (void *)&idx, SIZE_OF_IDX_CHK);

        if (idx == SECTION_EMPTY_CHK)
        {
            find_it = B_TRUE;
            break;
        }
    }

    if (find_it == B_FALSE)
    {
        eep_write_addr_ptr = CONFIG_EVENT_LOG_FIRST_PAGE_ADDR;
        eep_read_addr_ptr  = CONFIG_EVENT_LOG_FIRST_PAGE_ADDR;

        is_flash_data_exist = B_FALSE;
    }
    else
    {
        ptr_section = ptr_page;

        for (uint16_t i = 0; i < CONFIG_SECTION_NO; i++)
        {
            flash_Read(ptr_section, (void *)&idx, SIZE_OF_IDX_CHK);

            if (idx == SECTION_EMPTY_CHK)
            {
                if (i == CONFIG_SECTION_END)
                {
                    eep_read_addr_ptr = ptr_section;

                    if (ptr_page == CONFIG_EVENT_LOG_LAST_PAGE_ADDR)
                    {
                        eep_write_addr_ptr = CONFIG_EVENT_LOG_FIRST_PAGE_ADDR;
                    }
                    else
                    {
                        eep_write_addr_ptr = ptr_section + CONFIG_SECTION_SIZE;
                    }
                }
                else
                {
                    ptr_section += CONFIG_SECTION_SIZE;
                }
            }
            else
            {
                eep_write_addr_ptr = ptr_section;
                eep_read_addr_ptr  = ptr_section - CONFIG_SECTION_SIZE;
            }
        }

        is_flash_data_exist = B_TRUE;
    }
}
```

- **after**

```c
static inline bool_t SearchValidPtrForDataLog(void)
{
    bool_t       is_page_section_full = B_TRUE;
    bool_t       is_data_log_empty    = B_TRUE;
    uint64_t     idx                  = 0;
    flash_size_t ptr_page             = CONFIG_DATA_LOG_FIRST_PAGE_ADDR;
    flash_size_t ptr_section;

    /* search page*/
    for (uint16_t i = 0; i < CONFIG_PAGE_NO; i++)
    {
        flash_Read(ptr_page, (void *)&idx, SIZE_OF_IDX_CHK);
        if (idx == ACTIVE_SECTION_KEY)
        {
            is_data_log_empty = B_FALSE;
            break;
        }

        ptr_page += CONFIG_PAGE_SIZE;
    }

    if (is_data_log_empty)
    {
        eep_write_addr_ptr = CONFIG_DATA_LOG_FIRST_PAGE_ADDR;
        eep_read_addr_ptr  = CONFIG_DATA_LOG_FIRST_PAGE_ADDR;

        return B_FALSE;
    }

    /* search section*/
    ptr_section = ptr_page;

    for (uint16_t i = 1; i < CONFIG_SECTION_NO; i++)
    {
        ptr_section += CONFIG_SECTION_SIZE;

        flash_Read(ptr_section, (void *)&idx, SIZE_OF_IDX_CHK);
        if (idx != ACTIVE_SECTION_KEY)
        {
            is_page_section_full = B_FALSE;
            break;
        }
    }

    eep_read_addr_ptr = ptr_section - CONFIG_SECTION_SIZE;

    if (is_page_section_full && (ptr_page == CONFIG_DATA_LOG_LAST_PAGE_ADDR))
    {
        eep_write_addr_ptr = CONFIG_DATA_LOG_FIRST_PAGE_ADDR;
    }
    else
    {
        eep_write_addr_ptr = ptr_section;
    }

    return B_TRUE;
}
```

[table of content](#table-of-content)

## Example 2

- **before**

```c
bool_t bspFlash_WriteEventLog(const uint8_t *src)
{
    flash_size_t ptr_page           = CONFIG_EVENT_LOG_FIRST_PAGE_ADDR;
    flash_size_t eep_erase_addr_ptr = CONFIG_EVENT_LOG_FIRST_PAGE_ADDR;
    bool_t       need_to_erase      = B_FALSE;
    uint64_t     idx_chk            = SECTION_EMPTY_CHK;

    for (uint16_t i = 0; i < CONFIG_PAGE_NO; i++)
    {
        if (i > 0)
        {
            ptr_page += CONFIG_PAGE_SIZE;
        }

        if (eep_write_addr_ptr == ptr_page)
        {
            if (ptr_page == CONFIG_EVENT_LOG_FIRST_PAGE_ADDR)
            {
                eep_erase_addr_ptr = CONFIG_EVENT_LOG_LAST_PAGE_ADDR;
            }
            else
            {
                eep_erase_addr_ptr = eep_write_addr_ptr - CONFIG_PAGE_SIZE;
            }

            need_to_erase = B_TRUE;
            break;
        }
    }

    flash_Unlock();

    if (need_to_erase == B_TRUE)
    {
        if (flash_ErasePage(eep_erase_addr_ptr) == FLASH_ERROR)
        {
            return B_FALSE;
        }
    }

    if (flash_Write(eep_write_addr_ptr, (void *)&idx_chk, SIZE_OF_IDX_CHK) == FLASH_ERROR)
    {
        return B_FALSE;
    }

    if (flash_Write(eep_write_addr_ptr + SIZE_OF_IDX_CHK, src, CONFIG_EVENT_LOG_SIZE) ==
        FLASH_ERROR)
    {
        return B_FALSE;
    }

    flash_Lock();

    eep_read_addr_ptr = eep_write_addr_ptr;
    eep_write_addr_ptr += CONFIG_SECTION_SIZE;

    if (eep_write_addr_ptr == EVENT_LOG_END_ADDR)
    {
        eep_write_addr_ptr = CONFIG_EVENT_LOG_FIRST_PAGE_ADDR;
    }

    return B_TRUE;
}
```

- **after**

```c

static inline bool_t FlashWriteDataLog(const flash_size_t write_addr, const uint8_t *src)
{
    uint64_t     idx_chk       = ACTIVE_SECTION_KEY;
    flash_size_t idx_chk_addr  = write_addr;
    flash_size_t data_log_addr = write_addr + SIZE_OF_IDX_CHK;

    if (flash_Write(idx_chk_addr, (void *)&idx_chk, SIZE_OF_IDX_CHK) == FLASH_ERROR)
    {
        return B_FALSE;
    }

    if (flash_Write(data_log_addr, src, CONFIG_DATA_LOG_SIZE) == FLASH_ERROR)
    {
        return B_FALSE;
    }

    return B_TRUE;
};

static inline bool_t ChkDataLogPageChange(void)
{
    flash_size_t next_page_addr = CONFIG_DATA_LOG_FIRST_PAGE_ADDR;
    flash_size_t erase_page_ptr = PAGE_ADDR_NOT_SUPPORT;

    for (uint16_t i = 0; i < CONFIG_PAGE_NO; i++)
    {
        if (next_page_addr == eep_write_addr_ptr)
        {
            if (next_page_addr == CONFIG_DATA_LOG_FIRST_PAGE_ADDR)
            {
                erase_page_ptr = CONFIG_DATA_LOG_LAST_PAGE_ADDR;
            }
            else
            {
                erase_page_ptr = next_page_addr - CONFIG_PAGE_SIZE;
            }

            break;
        }

        next_page_addr += CONFIG_PAGE_SIZE;
    }

    if (erase_page_ptr != PAGE_ADDR_NOT_SUPPORT)
    {
        flash_Unlock();
        if (flash_ErasePage(erase_page_ptr) == FLASH_ERROR)
        {
            return B_FALSE;
        }
        flash_Lock();
    }

    return B_TRUE;
}

bool_t bspFlash_WriteDataLog(const uint8_t *src)
{
    bool_t is_write_success;
    bool_t is_page_change_success;

    is_page_change_success = ChkDataLogPageChange();

    if (!is_page_change_success)
    {
        return B_FALSE;
    }

    flash_Unlock();
    is_write_success = FlashWriteDataLog(eep_write_addr_ptr, src);
    flash_Lock();

    if (is_write_success)
    {
        eep_read_addr_ptr = eep_write_addr_ptr;

        eep_write_addr_ptr += CONFIG_SECTION_SIZE;
        if (eep_write_addr_ptr == DATA_LOG_END_ADDR)
        {
            eep_write_addr_ptr = CONFIG_DATA_LOG_FIRST_PAGE_ADDR;
        }
    }

    return is_write_success;
}
```

[table of content](#table-of-content)

## Example 3

- **before**

```c
void nvm_Init(void)
{
    if (bspFlash_IsFlashDataExist())
    {
        bspFlash_ReadEventLog((uint8_t *)&nvm_data.black_box);
    }

    bspFlash_ReadCalib((uint8_t *)&nvm_data.calib, sizeof(nvm_data.calib));

    calib_SanityChk((void *)&nvm_data.calib, sizeof(nvm_data.calib));

    calib_ConfigCalibDataPtr(&nvm_data.calib);
}
```

- **after**

```c
void calib_Init(void)
{
    CalibDataModuleInit();
    SanityChk();
}

bool_t bspFlash_ReadDataLog(uint8_t *dest)
{
    bool_t       is_read_success = B_FALSE;
    flash_size_t data_log_addr   = eep_read_addr_ptr + SIZE_OF_IDX_CHK;

    if (is_data_log_exist)
    {
        flash_Read(data_log_addr, dest, CONFIG_DATA_LOG_SIZE);
    }

    return is_read_success;
}

void nvm_Init(void)
{
    bspFlash_ReadDataLog((uint8_t *)&nvm_data.black_box);
    bspFlash_ReadCalib((uint8_t *)&nvm_data.calib, sizeof(nvm_data.calib));

    calib_ConfigCalibDataPtr(&nvm_data.calib);
}

```

[table of content](#table-of-content)

## Example 4  (naming)

- **before**

```c
static inline float ApplyCalib(calib_data_list_t index, float rms_avg)
{
    float           calib_rms_avg = 0;
    uint16_t        module_index  = 0;
    linear_calib_t *src           = (linear_calib_t *)calib_data;
    uint8_t         len           = calib_data_num_list[index];
    uint8_t         cnt           = 0;

    module_index = calib_SearchCalibModuleIndex(index);
    src          = src + module_index;

    while (rms_avg > src->measure_point.f && cnt < (len - 1))
    {
        src++;
        cnt++;
    }

    calib_rms_avg = rms_avg * src->gain.f + src->offset.f;

    return calib_rms_avg;
}

float calib_ApplyCalibVoutSetpoint(float rms_avg)
{
    return ApplyCalib(CALIB_INDEX_VOUT_SETPOINT, rms_avg);
}
```

- **after**

```c
static inline float GetCalculData(calib_data_list_t index, float rms_avg)
{
    float           calib_rms_avg = 0;
    linear_calib_t *src           = calib_data_module[index];
    uint8_t         len           = calib_data_num_list[index];

    for (uint16_t i = 0; i < len; i++)
    {
        if (rms_avg < src->measure_point.f)
        {
            break;
        }

        src++;
    }

    calib_rms_avg = rms_avg * src->gain.f + src->offset.f;

    return calib_rms_avg;
}

float calib_GetCalculVoutSetpoint(float rms_avg)
{
    return GetCalculData(CALIB_INDEX_VOUT_SETPOINT, rms_avg);
}
```

[table of content](#table-of-content)

## Example 5

- **before**

```c
static inline void SrCtrlUpd(uint16_t i_sense, uint16_t v_sense)
{
    static uint32_t dly_cnt = 0;

    if (bspHrtim_IsOutEnable())
    {
        if (i_sense <= sensor_ConvIoutToAdc(CONFIG_SR_OFF_THRES) ||
            v_sense <= sensor_ConvVanodeToAdc(CONFIG_SR_ON_VOLT_THRES))
        {
            dio_TurnOffSync1();
            dio_TurnOffSync2();
            dly_cnt = 0;
        }
        else
        {
            if (i_sense >= sensor_ConvIoutToAdc(CONFIG_SR_ON_THRES_2))
            {
                dio_TurnOnSync1();

                if (phase_shed_status)
                {
                    dio_TurnOnSync2();
                }
            }
            else if (i_sense >= sensor_ConvIoutToAdc(CONFIG_SR_ON_THRES_1))
            {
                dly_cnt++;
                if (dly_cnt >= SR_DLY_ON_CNT)
                {
                    dly_cnt = CONFIG_SR_DLY_ON_TIME;
                    dio_TurnOnSync1();

                    if (phase_shed_status)
                    {
                        dio_TurnOnSync2();
                    }
                }
            }
            else
            {
                dly_cnt = 0;
            }
        }
    }
    else
    {
        dio_TurnOffSync1();
        dio_TurnOffSync2();
        dly_cnt = 0;
    }
}
```

- **after**

```c

static inline void TurnOffSyncFet(void)
{
    dio_TurnOffSync1();
    dio_TurnOffSync2();
}

static inline void TurnOnSyncFet(void)
{
    dio_TurnOnSync1();

    if (phase_shed_status)
    {
        dio_TurnOnSync2();
    }
}

static inline void SrCtrlUpd(uint16_t i_sense, uint16_t v_sense)
{
    static uint32_t dly_cnt = 0;

    if (!bspHrtim_IsOutEnable() || (i_sense <= sensor_ConvIoutToAdc(CONFIG_SR_OFF_THRES) ||
                                    v_sense <= sensor_ConvVanodeToAdc(CONFIG_SR_ON_VOLT_THRES)))
    {
        TurnOffSyncFet();
        dly_cnt = 0;

        return;
    }

    if (i_sense >= sensor_ConvIoutToAdc(CONFIG_SR_ON_THRES_2) || dly_cnt >= SR_DLY_ON_CNT)
    {
        TurnOnSyncFet();
        dly_cnt = SR_DLY_ON_CNT;
    }
    else if (i_sense >= sensor_ConvIoutToAdc(CONFIG_SR_ON_THRES_1))
    {
        dly_cnt++;
    }
    else
    {
        dly_cnt = 0;
    }
}

```

[table of content](#table-of-content)

## Example 6

- **before**

```c
static inline void MainLoop(uint16_t vanode, uint16_t iout, uint16_t isense_1, uint16_t isense_2)
{
#if TEST_OPEN_LOOP == true
#if TEST_PEAK_CURRENT_OPEN_LOOP == true
    bspDac_SetPeakCurrentDacOut(GetI2cPeakCurrentCmd());
#else
    /* Set peak limit at 100A for protection */
    bspDac_SetPeakCurrentDacOut(sensor_ConvIoutToPriCtAdc(100));
    bspHrtim_ITTF1_DutyUpd(GetI2cDebugDuty());
#endif
#else

    VoLoop(vanode);
    CcLoop(iout);
#endif

    SideTaskInCtrlLoop(iout, vanode);
}
```

- **after**

```c
#if TEST_OPEN_LOOP == true
static inline void MainTaskInCtrlLoop(uint16_t vanode, uint16_t iout)
{
#if TEST_PEAK_CURRENT_OPEN_LOOP == true
    bspDac_SetPeakCurrentDacOut(GetI2cPeakCurrentCmd());
#else
    /* Set peak limit at 100A for protection */
    bspDac_SetPeakCurrentDacOut(sensor_ConvIoutToPriCtAdc(100));
    bspHrtim_ITTF1_MaxDutyUpd(GetI2cDebugDuty());
#endif
}
#else /* close loop relate function*/
static inline void VoLoop(uint16_t vanode)
{...}

static inline void CcLoop(uint16_t iout)
{...}

static inline void MainTaskInCtrlLoop(uint16_t vanode, uint16_t iout)
{
    VoLoop(vanode);
    CcLoop(iout);
}
#endif

void MainLoop(uint16_t vanode, uint16_t iout, uint16_t isense_1, uint16_t isense_2)
{
    MainTaskInCtrlLoop(vanode, iout);
    SideTaskInCtrlLoop(iout, vanode);
}
```

[table of content](#table-of-content)


## Example 7

- **before**

```c
static inline void SideTaskInCtrlLoop(uint16_t i_sense, uint16_t v_sense)
{
    static uint8_t freq_divider = 0;

    freq_divider++;

    if (freq_divider % 2)
    {
#if TEST_OPEN_LOOP == false
#if TEST_DISABLE_SYNC_FET == false
        SrCtrlUpd(i_sense, v_sense);
#endif

#if TEST_DISABLE_PHASE_SHED == false
        PhaseShedCtrlUpd(i_sense);
#endif
#endif
    }
    else
    {
        StepUpVref(&v_ref, i_sense);
    }
}
```

- **after**

```c
#if TEST_DISABLE_PHASE_SHED == false
void PhaseShedCtrlUpd(uint16_t i_sense, bool_t *is_phase_shed)
{...}
#else
void PhaseShedCtrlUpd(uint16_t i_sense, bool_t *is_phase_shed)
{;}
#endif

#if TEST_DISABLE_SYNC_FET == false
static inline void SrCtrlUpd(uint16_t i_sense, uint16_t v_sense)
{...}
#else
static inline void SrCtrlUpd(uint16_t i_sense, uint16_t v_sense)
{;}
#endif

static inline void SideTaskInCtrlLoop(uint16_t i_sense, uint16_t v_sense)
{
    static uint8_t freq_divider = 0;

    freq_divider++;

    if (freq_divider % 2)
    {
#if TEST_OPEN_LOOP == false
        SrCtrlUpd(i_sense, v_sense);
        ctrl_loop[MODEL_SELECT].PhaseShedCtrlUpd(i_sense, &is_phase_shed);
#endif
    }
    else
    {
        StepUpVref(&v_ref, i_sense);
    }
}
```

[table of content](#table-of-content)

## Example 8

- **before**

```c
#define FULL_SINE_WAVE      B_TRUE
#define RECTIFIED_SINE_WAVE B_FALSE

/**
 * @name helper function for sine pakage initialization
 * e.g.:
 * @code{.c}
 * static sine_pkg_t vac_virtual = sine_PkgInit(50 HZ, 65KHz, FULL_SINE_WAVE)
 * @endcode
 * @{
 */
#define sine_PkgInit(freq, execute_freq, is_full_sine_wave_en)                                \
    {                                                                                         \
        (uint32_t)((float)execute_freq / (float)freq / 4),                                    \
            (uint32_t)((float)execute_freq / (float)freq / 2),                                \
            (uint32_t)((float)execute_freq / (float)freq), (uint32_t)0, is_full_sine_wave_en, \
            (float)0, sine_Process,                                                           \
    }

void sine_Process(sine_pkg_t *sine_pkg)
{
    float    tmp        = 0;
    uint32_t corr_cnt   = 0;
    uint32_t sine_index = 0;

    // Update step
    if (sine_pkg->cnt < sine_pkg->half)
    {
        if (sine_pkg->cnt <= sine_pkg->qtr)
        {
            corr_cnt = sine_pkg->cnt;
        }
        else
        {
            corr_cnt = sine_pkg->half - sine_pkg->cnt;
        }
    }
    else
    {
        if ((sine_pkg->cnt - sine_pkg->half) <= sine_pkg->qtr)
        {
            corr_cnt = sine_pkg->cnt - sine_pkg->half;
        }
        else
        {
            corr_cnt = (sine_pkg->period - 1) - sine_pkg->cnt;
        }
    }

    tmp        = corr_cnt * div_table[sine_pkg->qtr];
    /* Multiply by 1023 is for mapping to the sine_table size  */
    sine_index = (uint32_t)(tmp * 1023);

    sine_pkg->sine_val = sine_table[sine_index];

    if (sine_pkg->is_full_sine_wave_en)
    {
        if (sine_pkg->cnt >= sine_pkg->half)
        {
            sine_pkg->sine_val = -sine_pkg->sine_val;
        }
    }

    /* The period shall minus 1 e.g. cnt 200 the effective is 0~199 (0 equal to 200)*/
    if (sine_pkg->cnt < (sine_pkg->period - 1))
    {
        sine_pkg->cnt++;
    }
    else
    {
        sine_pkg->cnt = 0;
    }
}
```

- **after**

```c
typedef enum sine_wave_type_e
{
    FULL_SINE_WAVE      = 0,
    RECTIFIED_SINE_WAVE = 1
} sine_wave_type_t;

/** @name helper function for sine package initialization
 *
 * @param[in] freq frequency of generated sine wave
 * @param[in] execute_freq frequency of sine wave update
 * @param[in] sine_wave_type `FULL_SINE_WAVE` or `RECTIFIED_SINE_WAVE`
 *
 * @par example
 *
 * @code{.c}
 * static sine_pkg_t vac_virtual = sine_PkgDeclareInit(50 HZ, 65KHz, FULL_SINE_WAVE)
 * @endcode
 * @{*/
#define sine_PkgDeclareInit(freq, execute_freq, sine_wave_type)                                   \
    {                                                                                             \
        (uint32_t)((float)execute_freq / (float)freq / 4),                                        \
            (uint32_t)((float)execute_freq / (float)freq / 2),                                    \
            (uint32_t)((float)execute_freq / (float)freq), (uint32_t)0, sine_wave_type, (float)0, \
    }

void sine_Process(sine_pkg_t *sine_pkg)
{
    float    corr_normalized = 0;
    uint32_t corr_cnt        = 0;
    uint32_t sine_index      = 0;
    uint32_t degree_90_cnt   = sine_pkg->qtr;
    uint32_t degree_180_cnt  = sine_pkg->half;
    uint32_t degree_270_cnt  = sine_pkg->qtr + sine_pkg->qtr;
    uint32_t degree_360_cnt  = sine_pkg->period;

    if (sine_pkg->cnt <= degree_90_cnt)
    {
        corr_cnt = sine_pkg->cnt;
    }
    else if (sine_pkg->cnt <= degree_180_cnt)
    {
        corr_cnt = degree_180_cnt - sine_pkg->cnt;
    }
    else if (sine_pkg->cnt <= degree_270_cnt)
    {
        corr_cnt = sine_pkg->cnt - degree_180_cnt;
    }
    else
    {
        corr_cnt = degree_360_cnt - sine_pkg->cnt;
    }

    /* For mapping to the qtr_sine_table size */
    corr_normalized = corr_cnt * divisor_table[sine_pkg->qtr];
    sine_index      = (uint32_t)(corr_normalized * QTR_SINE_TABLE_INDEX_MAX);

    if (sine_pkg->sine_wave_type == FULL_SINE_WAVE)
    {
        if (sine_pkg->cnt >= degree_180_cnt)
        {
            sine_pkg->sine_val = -qtr_sine_table[sine_index];
        }
        else
        {
            sine_pkg->sine_val = qtr_sine_table[sine_index];
        }
    }
    else if (sine_pkg->sine_wave_type == RECTIFIED_SINE_WAVE)
    {
        sine_pkg->sine_val = qtr_sine_table[sine_index];
    }

    /* update cnt*/
    sine_pkg->cnt++;
    if (sine_pkg->cnt == degree_360_cnt)
    {
        sine_pkg->cnt = 0;
    }
}
```

[table of content](#table-of-content)

## Example 9

- **before**

```c
bool_t PmbusDroop(pmbus_handle_args_t *args, pmbus_error_t *p_ret_val)
{
    if (args->is_read)
    {
        uint16_t droop = 0;

        if (args->ph->current_page)
        {
            droop = pmbus_FloatToLinear11AutoExp(stbyRail_GetDroopRatio(), CONFIG_DROOP_MIN_N);
        }
        else
        {
            droop = pmbus_FloatToLinear11AutoExp(ishare_GetDroopRatio(), CONFIG_DROOP_MIN_N);
        }

        uint8_t out[CONFIG_CMD_28_DATA_LEN] = {cfp_LowByte(droop), cfp_HighByte(droop)};
        pmbus_SendResponse(args->ph, out, CONFIG_CMD_28_DATA_LEN);
    }
    else
    {
        float droop =
            pmbus_Linear11ToFloat(cfp_Fetch16Le((uint8_t *)&args->rx_data[PMBUS_DATA_INDEX]));

        if (args->ph->current_page)
        {
            stbyRail_SetDroopRatio(droop);
        }
        else
        {
            ishare_SetDroopRatio(droop);
        }
    }
    return B_TRUE;
}
```

- **after**

```c
bool_t PmbusDroop(pmbus_handle_args_t *args, pmbus_error_t *p_ret_val)
{
    uint8_t    current_page = args->ph->current_page;
    float      droop_ratio;
    uint8_t    out[CONFIG_CMD_28_DATA_LEN];
    linear11_t droop_ratio_linear11 = 0;

    if (args->is_read)
    {
        switch (current_page)
        {
            case 0:
                droop_ratio = ishare_GetDroopRatio();
                break;
            case 1:
            default:
                droop_ratio = stbyRail_GetDroopRatio();
                break;
        }

        droop_ratio_linear11 = pmbus_FloatToLinear11AutoExp(droop_ratio, CONFIG_DROOP_MIN_N);
        cfp_Store16Le(out, droop_ratio_linear11);

        pmbus_SendResponse(args->ph, out, CONFIG_CMD_28_DATA_LEN);
    }
    else
    {
        droop_ratio_linear11 = cfp_Fetch16Le(&args->rx_data[PMBUS_DATA_INDEX]);
        droop_ratio          = pmbus_Linear11ToFloat(droop_ratio_linear11);

        switch (current_page)
        {
            case 0:
                ishare_SetDroopRatio(droop_ratio);
                break;
            case 1:
            default:
                stbyRail_SetDroopRatio(droop_ratio);
                break;
        }
    }
    return B_TRUE;
}
```

[table of content](#table-of-content)

## Example 10

- **before**

```c
#define CONFIG_SAWTOOTH_DAC_STEP                                            \
    (uint32_t)(ITTF_PCMODE_SLOPE_COMPENS * 16 / REF_VREF * (float)ADC_RES * \
               ((float)CONFIG_ITTF_1_HRTIM_CMP2 / (float)CONFIG_HRTIMER_FREQUENCY))

```

- **after**

```c
#define DAC_MAX_SAWTOOTH_INC_VALUE (uint16_t)(0xFFFFU)
#define PC_MODE_SLOPE_CNT           ((ITTF_PCMODE_SLOPE_COMPENS / REF_VREF) * DAC_MAX_SAWTOOTH_INC_VALUE)

#define CONFIG_SAWTOOTH_DAC_STEP  (uint16_t)(PC_MODE_SLOPE_CNT * CONFIG_SAWTOOTH_UPDATE_PERIOD)
```

[table of content](#table-of-content)

## Example 11

- **before**

```c
typedef struct first_order_flt_group_s
{
    float    accum;
    float    avg_output;
    uint32_t len;
} first_order_flt_group_t;

#define tfp_FltFreqToTau(freq) (float)(1 / (2 * PI * freq))

/** @name helper function for first order filter group initialization with τ fromat
 *
 * @param[in] tau_time tau = τ = RC = (1 / 2πf) presents the charactersic of 1st low pass filter
 * @param[in] execute_freq frequency of sine wave update
 *
 * @par example
 *
 * @code{.c}
 * static first_order_flt_group_t vac_flt = tfp_FirstOrderFltDeclareInit(0.05, 65 KHZ)
 * static first_order_flt_group_t vac_flt =
 *               tfp_FirstOrderFltDeclareInit(tfp_FltFreqToTau(60 HZ), 65 KHZ)
 * @endcode
 * @{*/
#define tfp_FirstOrderFltDeclareInit(tau_time, execute_freq)    \
    {                                                           \
        (float)0, (float)0, (uint32_t)(tau_time * execute_freq) \
    }
/** @}*/

float tfp_FirstOrderFlt(float avg_input, first_order_flt_group_t *src);

float tfp_FirstOrderFlt(float avg_input, first_order_flt_group_t *src);

/**
 * @brief divisor look up table, (range 1~DIVISOR_TABLE_INDEX_MAX)
 * @note don't use divisor_table[0] because divisor cannot be 0.
 * @par example: result = 5.1 / 300
 *
 * @code{.c}
 *  float result = 0;
 *  float dividend = 5.1;
 *  int divisor = 300;
 *
 *  result = dividend * divisor_table[300];
 * @endcode
 */
extern float divisor_table[DIVISOR_TABLE_SIZE];

float tfp_FirstOrderFlt(float avg_input, first_order_flt_group_t *src)
{
    src->accum += avg_input;
    src->accum -= src->avg_output;
    src->avg_output = src->accum * divisor_table[src->len];

    return src->avg_output;
}
```

- **after**

```c
/** @name helper function for first order low pass filter group initialization with τ fromat
 * @par example
 *
 * @code{.c}
 * #define VAC_FLT_TAU = tfp_GetLowpassFltTau(60 Hz)
 *
 * static lowpass_flt_group_t vac_flt = tfp_LowpassFltDeclareInit(0.05, 65 KHZ)
 * static lowpass_flt_group_t vac_flt = tfp_LowpassFltDeclareInit(VAC_FLT_TAU, 65 KHZ)
 * ...
 * update_area(float target_vac)
 * {
 *     avg_target_vac = tfp_GetLowpassFltResult(target_vac, &vac_flt);
 * }
 *
 * @endcode
 * @{*/
typedef struct lowpass_flt_group_s
{
    float    accum;
    float    out;
    uint32_t len;
} lowpass_flt_group_t;

/** @param[in] f_c : cutoff frequency*/
#define tfp_GetLowpassFltTau(f_c) (float)(1 / tfp_FreqInRad(f_c))

/**
 * @param[in] tau : τ = RC = 1 / (2 π * f_c) presents the charactersic
 * of 1st low pass filter, where f_c is cutoff frequency
 * @param[in] f_s sampling frequency
 *
 * @warning make sure that "tau * f_s" is smaller than `DIVISOR_TABLE_SIZE`,
 * Because we use look up table for division.
 */
#define tfp_LowpassFltDeclareInit(tau, f_s) \
    {                                       \
        .len = (uint32_t)(tau * f_s),       \
    }

float tfp_GetLowpassFltOut(float input, lowpass_flt_group_t *src);
/** @}*/

/**
 * @brief look up table version of division
 *
 * @param[in] dividend float type number
 * @param[in] divisor uint type number, from 1 ~ DIVISOR_TABLE_INDEX_MAX
 */
#define tfp_Divsion(dividend, divisor) ((dividend)*divisor_table[(divisor)])
/** @}*/

float tfp_GetLowpassFltOut(float input, lowpass_flt_group_t *src)
{
    float prev_out = src->out;

    src->accum += input;
    src->accum -= prev_out;
    src->out = tfp_Divsion(src->accum, src->len);

    return src->out;
}
```

[table of content](#table-of-content)

## Example 12

- **before**

```c

```

- **after**

```c

```

[table of content](#table-of-content)
