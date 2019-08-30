#include <stdint.h>
#include <target/target.h>
#include <target/target_type.h>
#include <helper/log.h>

static int leon3_poll(struct target *target) {
        return ERROR_OK;
}

static int leon3_arch_state(struct target *target) {
  return ERROR_OK;
}

static int leon3_halt(struct target *target) {
  return ERROR_OK;
}

static int leon3_resume(struct target *target, int current, target_addr_t address,
			int handle_breakpoints, int debug_execution) {
  return ERROR_OK;
}

static int leon3_step(struct target *target, int current, target_addr_t address,
                      int handle_breakpoints) {
  return ERROR_OK;
}

static int leon3_assert_reset(struct target *target) {
  return ERROR_OK;
}

static int leon3_deassert_reset(struct target *target) {
  return ERROR_OK;
}

static int leon3_soft_reset_halt(struct target *target) {
  return ERROR_OK;
}

static int leon3_get_gdb_reg_list(struct target *target, struct reg **reg_list[],
                                  int *reg_list_size, enum target_register_class reg_class) {
  return ERROR_OK;
}

static int leon3_read_memory(struct target *target, target_addr_t address,
                             uint32_t size, uint32_t count, uint8_t *buffer) {
  LOG_DEBUG("leon3_read_memory");
  return ERROR_OK;
}

static int leon3_write_memory(struct target *target, target_addr_t address,
                              uint32_t size, uint32_t count, const uint8_t *buffer) {
  return ERROR_OK;
}

static int leon3_target_create(struct target *target, Jim_Interp *interp) {
  LOG_INFO("leon3_target_create");
  return ERROR_OK;
}

static int leon3_examine(struct target *target) {
  if (! target_was_examined(target)) {
    target_set_examined(target);
  }
  return ERROR_OK;
}

static int leon3_init_target(struct command_context *cmd_ctx, struct target *target) {
  LOG_INFO("leon3_init_target");
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
