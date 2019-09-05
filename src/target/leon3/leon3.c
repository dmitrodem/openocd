#include <stdint.h>
#include <target/target.h>
#include <target/target_type.h>
#include <target/register.h>
#include <helper/log.h>
#include <jtag/jtag.h>

#define LEON3_AINST 0x2
#define LEON3_DINST 0x3

#define DSU_BASE 0x90000000

struct leon3_core_reg_init {
        const char *name;
        uint32_t list_num; /* Index in register cache */
        const char *feature;
        const char *group;
};

static const struct leon3_core_reg_init leon3_init_reg_list[] = {
        {"g0"  , 0,  "org.gnu.gdb.sparc.cpu", "cpu"},
        {"g1"  , 1,  "org.gnu.gdb.sparc.cpu", "cpu"},
        {"g2"  , 2,  "org.gnu.gdb.sparc.cpu", "cpu"},
        {"g3"  , 3,  "org.gnu.gdb.sparc.cpu", "cpu"},
        {"g4"  , 4,  "org.gnu.gdb.sparc.cpu", "cpu"},
        {"g5"  , 5,  "org.gnu.gdb.sparc.cpu", "cpu"},
        {"g6"  , 6,  "org.gnu.gdb.sparc.cpu", "cpu"},
        {"g7"  , 7,  "org.gnu.gdb.sparc.cpu", "cpu"},
        {"o0"  , 8,  "org.gnu.gdb.sparc.cpu", "cpu"},
        {"o1"  , 9,  "org.gnu.gdb.sparc.cpu", "cpu"},
        {"o2"  , 10, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"o3"  , 11, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"o4"  , 12, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"o5"  , 13, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"sp"  , 14, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"o7"  , 15, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"l0"  , 16, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"l1"  , 17, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"l2"  , 18, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"l3"  , 19, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"l4"  , 20, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"l5"  , 21, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"l6"  , 22, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"l7"  , 23, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"i0"  , 24, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"i1"  , 25, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"i2"  , 26, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"i3"  , 27, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"i4"  , 28, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"i5"  , 29, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"fp"  , 30, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"i7"  , 31, "org.gnu.gdb.sparc.cpu", "cpu"},
        {"f0"  , 32, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f1"  , 33, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f2"  , 34, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f3"  , 35, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f4"  , 36, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f5"  , 37, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f6"  , 38, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f7"  , 39, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f8"  , 40, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f9"  , 41, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f10" , 42, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f11" , 43, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f12" , 44, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f13" , 45, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f14" , 46, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f15" , 47, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f16" , 48, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f17" , 49, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f18" , 50, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f19" , 51, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f20" , 52, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f21" , 53, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f22" , 54, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f23" , 55, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f24" , 56, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f25" , 57, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f26" , 58, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f27" , 59, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f28" , 60, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f29" , 61, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f30" , 62, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"f31" , 63, "org.gnu.gdb.sparc.fpu", "fpu"},
        {"y"   , 64, "org.gnu.gdb.sparc.cp0", "cp0"},
        {"psr" , 65, "org.gnu.gdb.sparc.cp0", "cp0"},
        {"wim" , 66, "org.gnu.gdb.sparc.cp0", "cp0"},
        {"tbr" , 67, "org.gnu.gdb.sparc.cp0", "cp0"},
        {"pc"  , 68, "org.gnu.gdb.sparc.cp0", "cp0"},
        {"npc" , 69, "org.gnu.gdb.sparc.cp0", "cp0"},
        {"fsr" , 70, "org.gnu.gdb.sparc.cp0", "cp0"},
        {"csr" , 71, "org.gnu.gdb.sparc.cp0", "cp0"}
};

const size_t leon3_init_reg_list_size = sizeof(leon3_init_reg_list)/sizeof(leon3_init_reg_list[0]);

struct leon3_core_reg {
        struct target *target;
};

static int leon3_poll(struct target *target) {
  /* LOG_DEBUG(" "); */
  return ERROR_OK;
}

static int leon3_arch_state(struct target *target) {
  LOG_DEBUG(" ");
  return ERROR_OK;
}

static int leon3_halt(struct target *target) {
  LOG_DEBUG(" ");
  target->state = TARGET_HALTED;
  return ERROR_OK;
}

static int leon3_resume(struct target *target, int current, target_addr_t address,
                        int handle_breakpoints, int debug_execution) {
  LOG_DEBUG(" ");
  target->state = TARGET_RUNNING;
  return ERROR_OK;
}

static int leon3_step(struct target *target, int current, target_addr_t address,
                      int handle_breakpoints) {
  LOG_DEBUG(" ");
  return ERROR_OK;
}

static int leon3_assert_reset(struct target *target) {
  LOG_DEBUG(" ");
  return ERROR_OK;
}

static int leon3_deassert_reset(struct target *target) {
  LOG_DEBUG(" ");
  return ERROR_OK;
}

static int leon3_soft_reset_halt(struct target *target) {
  LOG_DEBUG(" ");
  return ERROR_OK;
}

static int leon3_get_gdb_reg_list(struct target *target, struct reg **reg_list[],
                                  int *reg_list_size, enum target_register_class reg_class) {
  LOG_DEBUG("reg_class = %i", (int) reg_class);
  *reg_list = malloc(leon3_init_reg_list_size * sizeof(struct reg *));

  struct reg_feature *feature;
  size_t i;
  for (i = 0; i < leon3_init_reg_list_size; i++) {
          struct reg *r = malloc(sizeof(struct reg));
          register_init_dummy(r);
          r->name = leon3_init_reg_list[i].name;
          r->number = i;
          feature = malloc(sizeof(struct reg_feature));
          feature->name = leon3_init_reg_list[i].feature;
          r->feature = feature;
          r->caller_save = false;
          r->value = calloc(1, 4);
          buf_set_u32(r->value, 0, 32, 100+i);
          r->dirty = true;
          r->valid = false;
          r->exist = true;
          r->size = 32;
          r->reg_data_type = NULL;
          r->group = NULL;
          r->arch_info = target;
          (*reg_list)[i] = r;
  }
  *reg_list_size = leon3_init_reg_list_size;
  return ERROR_OK;
}

static int leon3_read_memory(struct target *target, target_addr_t address,
                             uint32_t size, uint32_t count, uint8_t *buffer) {
  LOG_DEBUG("Read memory at 0x%08" TARGET_PRIxADDR ", size: %" PRIu32 ", count: 0x%08" PRIx32, address, size, count);

  /* Sanitize arguments */
  if ((size != 4) || (count == 0) || !buffer) {
    LOG_ERROR("Bad arguments");
    return ERROR_COMMAND_SYNTAX_ERROR;
  }

  if (((size == 4) && (address & 0x3u)) || ((size == 2) && (address & 0x1u))) {
    LOG_ERROR("Can't handle unaligned memory access");
    return ERROR_TARGET_UNALIGNED_ACCESS;
  }

  struct jtag_tap *tap = target->tap;
  struct scan_field field;
  uint8_t ir_value;

  ir_value = LEON3_AINST;
  field.num_bits = tap->ir_length;
  field.out_value = &ir_value;
  field.in_value = NULL;

  jtag_add_ir_scan(tap, &field, TAP_IDLE);

  uint8_t buf[5];
  h_u32_to_le(buf, address);
  buf[4] = 0x02;

  field.num_bits = 35;
  field.out_value = buf;
  field.in_value = NULL;

  jtag_add_dr_scan(tap, 1, &field, TAP_IDLE);

  ir_value = LEON3_DINST;
  field.num_bits = tap->ir_length;
  field.out_value = &ir_value;
  field.in_value = NULL;

  jtag_add_ir_scan(tap, &field, TAP_IDLE);

  jtag_execute_queue();

  size_t i;
  uint8_t outbuf[5];
  uint8_t  inbuf[5];
  for (i = 0; i < count; i++) {
    do {
      memset(outbuf, 0x00, sizeof(outbuf));
      outbuf[4] = (i == count - 1) ? 0 : 1;
      field.num_bits = 33;
      field.out_value = outbuf;
      field.in_value = inbuf;

      jtag_add_dr_scan(tap, 1, &field, TAP_IDLE);
      jtag_execute_queue();
    } while ((inbuf[4] & 0x01) != 0x01);

    buf_bswap32(&buffer[4*i], inbuf, 4);
  }

  return ERROR_OK;
}

static int leon3_write_memory(struct target *target, target_addr_t address,
                              uint32_t size, uint32_t count, const uint8_t *buffer) {
  LOG_DEBUG("Write memory at 0x%08" TARGET_PRIxADDR ", size: %" PRIu32 ", count: 0x%08" PRIx32, address, size, count);

  if (target->state != TARGET_HALTED) {
          LOG_WARNING("Target not halted");
          return ERROR_TARGET_NOT_HALTED;
  }

  /* Sanitize arguments */
  if (((size != 4) && (size != 2) && (size != 1)) || (count == 0) || !buffer) {
          LOG_ERROR("Bad arguments");
          return ERROR_COMMAND_SYNTAX_ERROR;
  }

  if (((size == 4) && (address & 0x3u)) || ((size == 2) && (address & 0x1u))) {
          LOG_ERROR("Can't handle unaligned memory access");
          return ERROR_TARGET_UNALIGNED_ACCESS;
  }

  struct jtag_tap *tap = target->tap;
  struct scan_field field;
  uint8_t ir_value;

  ir_value = LEON3_AINST;
  field.num_bits = tap->ir_length;
  field.out_value = &ir_value;
  field.in_value = NULL;

  jtag_add_ir_scan(tap, &field, TAP_IDLE);

  uint8_t buf[5];
  h_u32_to_le(buf, address);
  buf[4] = 0x04 | 0x02;

  field.num_bits = 35;
  field.out_value = buf;
  field.in_value = NULL;

  jtag_add_dr_scan(tap, 1, &field, TAP_IDLE);

  ir_value = LEON3_DINST;
  field.num_bits = tap->ir_length;
  field.out_value = &ir_value;
  field.in_value = NULL;

  jtag_add_ir_scan(tap, &field, TAP_IDLE);

  jtag_execute_queue();

  size_t i;
  uint8_t outbuf[5];
  uint8_t  inbuf[5];
  for (i = 0; i < count; i++) {
          if (target->endianness == TARGET_BIG_ENDIAN) {
                  buf_bswap32(outbuf, buffer + 4*i, 4);
          } else {
                  memcpy(outbuf, buffer + 4*i, 4);
          }
          outbuf[4] = (i == count - 1) ? 0 : 1;
          field.num_bits = 33;
          field.out_value = outbuf;
          field.in_value = inbuf;

          jtag_add_dr_scan(tap, 1, &field, TAP_IDLE);
          jtag_execute_queue();
  }


  return ERROR_OK;
}

static int leon3_target_create(struct target *target, Jim_Interp *interp) {
  LOG_DEBUG("tap: %s", jtag_tap_name(target->tap));
  return ERROR_OK;
}

static int leon3_examine(struct target *target) {
  LOG_DEBUG(" ");
  if (! target_was_examined(target)) {
    target_set_examined(target);
  }
  return ERROR_OK;
}

static int leon3_init_target(struct command_context *cmd_ctx, struct target *target) {
  LOG_DEBUG(" ");
  return ERROR_OK;
}

struct target_type leon3_target = {
        .name = "leon3",

        .poll = leon3_poll,
        .arch_state = leon3_arch_state,

        .target_request_data = NULL,

        .halt = leon3_halt,
        .resume = leon3_resume,
        .step = leon3_step,

        .assert_reset = leon3_assert_reset,
        .deassert_reset = leon3_deassert_reset,
        .soft_reset_halt = leon3_soft_reset_halt,

        .get_gdb_reg_list = leon3_get_gdb_reg_list,

        .read_memory = leon3_read_memory,
        .write_memory = leon3_write_memory,

        .target_create = leon3_target_create,
        .init_target = leon3_init_target,
        .examine = leon3_examine,

};


/* Local Variables: */
/* mode: c */
/* End: */
