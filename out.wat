(module
  (type (;0;) (func (param i32)))
  (type (;1;) (func (param i32 i32) (result i32)))
  (type (;2;) (func (param i32 i32 i32 i32) (result i32)))
  (type (;3;) (func (param i32 i32)))
  (type (;4;) (func (param i32 f32 i32)))
  (type (;5;) (func (param i32) (result i32)))
  (type (;6;) (func (result i32)))
  (import "env" "memory" (memory (;0;) 155))
  (import "env" "__indirect_function_table" (table (;0;) 1 funcref))
  (import "env" "wasm_print_line" (func $wasm_print_line (type 0)))
  (func $t__snprintf_parse_flag (type 1) (param i32 i32) (result i32)
    (local i32)
    i32.const -1
    local.set 2
    block  ;; label = @1
      local.get 1
      i32.const -32
      i32.add
      local.tee 1
      i32.const 16
      i32.gt_u
      br_if 0 (;@1;)
      block  ;; label = @2
        block  ;; label = @3
          block  ;; label = @4
            block  ;; label = @5
              block  ;; label = @6
                local.get 1
                br_table 1 (;@5;) 5 (;@1;) 5 (;@1;) 2 (;@4;) 5 (;@1;) 5 (;@1;) 5 (;@1;) 5 (;@1;) 5 (;@1;) 5 (;@1;) 5 (;@1;) 0 (;@6;) 5 (;@1;) 4 (;@2;) 5 (;@1;) 5 (;@1;) 3 (;@3;) 1 (;@5;)
              end
              local.get 0
              i32.const 4
              i32.add
              local.set 0
              br 3 (;@2;)
            end
            local.get 0
            i32.const 8
            i32.add
            local.set 0
            br 2 (;@2;)
          end
          local.get 0
          i32.const 12
          i32.add
          local.set 0
          br 1 (;@2;)
        end
        local.get 0
        i32.const 16
        i32.add
        local.set 0
      end
      i32.const 1
      local.set 2
      local.get 0
      i32.const 1
      i32.store
    end
    local.get 2)
  (func $format (type 2) (param i32 i32 i32 i32) (result i32)
    (local i32)
    global.get 0
    i32.const 64
    i32.sub
    local.tee 4
    global.set 0
    local.get 4
    local.get 2
    i32.store offset=60
    local.get 4
    local.get 3
    i32.store offset=56
    local.get 4
    local.get 0
    i32.store offset=48
    local.get 4
    i32.const 0
    i32.store offset=44
    local.get 4
    local.get 1
    i32.const -1
    i32.add
    i32.store offset=40
    block  ;; label = @1
      loop  ;; label = @2
        local.get 4
        local.get 4
        i32.load offset=60
        local.tee 1
        i32.const 1
        i32.add
        i32.store offset=60
        block  ;; label = @3
          local.get 1
          i32.load8_u
          local.tee 1
          i32.const 37
          i32.eq
          br_if 0 (;@3;)
          local.get 1
          i32.eqz
          br_if 2 (;@1;)
          local.get 4
          i32.const 40
          i32.add
          local.get 1
          i32.const 24
          i32.shl
          i32.const 24
          i32.shr_s
          call $t__snprintf_add_char
          br 1 (;@2;)
        end
        local.get 4
        i32.const 16
        i32.add
        local.get 4
        i32.const 60
        i32.add
        call $t__snprintf_parse_flags
        local.get 4
        i32.const 8
        i32.add
        local.get 4
        i32.const 60
        i32.add
        call $t__snprintf_parse_width
        block  ;; label = @3
          local.get 4
          i32.load offset=8
          br_if 0 (;@3;)
          local.get 4
          local.get 4
          i32.load offset=56
          local.tee 1
          i32.const 4
          i32.add
          i32.store offset=56
          local.get 4
          local.get 1
          i32.load
          i32.store offset=12
        end
        local.get 4
        i32.const 4
        i32.add
        local.get 4
        i32.const 60
        i32.add
        call $t__snprintf_parse_precision
        local.get 4
        local.get 4
        i32.load offset=60
        local.tee 1
        i32.const 1
        i32.add
        i32.store offset=60
        block  ;; label = @3
          local.get 1
          i32.load8_s
          local.tee 0
          i32.const -100
          i32.add
          local.tee 1
          i32.const 17
          i32.le_u
          br_if 0 (;@3;)
          local.get 0
          i32.const 37
          i32.ne
          br_if 1 (;@2;)
          local.get 4
          i32.const 40
          i32.add
          i32.const 37
          call $t__snprintf_add_char
          br 1 (;@2;)
        end
        block  ;; label = @3
          block  ;; label = @4
            block  ;; label = @5
              block  ;; label = @6
                local.get 1
                br_table 1 (;@5;) 4 (;@2;) 3 (;@3;) 4 (;@2;) 4 (;@2;) 1 (;@5;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 0 (;@6;) 4 (;@2;) 2 (;@4;) 1 (;@5;)
              end
              local.get 4
              local.get 4
              i32.load offset=56
              local.tee 1
              i32.const 4
              i32.add
              i32.store offset=56
              local.get 4
              i32.const 40
              i32.add
              local.get 1
              i32.load
              call $t__snprintf_add_string
              br 3 (;@2;)
            end
            local.get 4
            local.get 4
            i32.load offset=56
            local.tee 1
            i32.const 4
            i32.add
            i32.store offset=56
            block  ;; label = @5
              local.get 1
              i32.load
              local.tee 1
              i32.const 0
              i32.lt_s
              br_if 0 (;@5;)
              local.get 4
              i32.load offset=20
              i32.eqz
              br_if 0 (;@5;)
              local.get 4
              i32.const 40
              i32.add
              i32.const 43
              call $t__snprintf_add_char
            end
            local.get 4
            i32.const 40
            i32.add
            local.get 1
            call $t__snprintf_add_int
            br 2 (;@2;)
          end
          local.get 4
          local.get 4
          i32.load offset=56
          local.tee 1
          i32.const 4
          i32.add
          i32.store offset=56
          local.get 4
          i32.const 40
          i32.add
          local.get 1
          i32.load
          call $t__snprintf_add_uint
          br 1 (;@2;)
        end
        local.get 4
        local.get 4
        i32.load offset=56
        i32.const 7
        i32.add
        i32.const -8
        i32.and
        local.tee 1
        i32.const 8
        i32.add
        i32.store offset=56
        local.get 4
        i32.const 40
        i32.add
        local.get 1
        f64.load
        f32.demote_f64
        local.get 4
        i32.load offset=4
        call $t__snprintf_add_float
        br 0 (;@2;)
      end
    end
    block  ;; label = @1
      local.get 4
      i32.load offset=48
      local.tee 1
      i32.eqz
      br_if 0 (;@1;)
      local.get 1
      i32.const 0
      i32.store8
      local.get 4
      local.get 1
      i32.const 1
      i32.add
      i32.store offset=48
    end
    local.get 4
    i32.load offset=44
    local.set 1
    local.get 4
    i32.const 64
    i32.add
    global.set 0
    local.get 1)
  (func $t__snprintf_add_char (type 3) (param i32 i32)
    (local i32 i32)
    block  ;; label = @1
      local.get 0
      i32.load
      local.tee 2
      i32.const 1
      i32.lt_s
      br_if 0 (;@1;)
      local.get 0
      i32.load offset=8
      local.tee 3
      i32.eqz
      br_if 0 (;@1;)
      local.get 0
      local.get 3
      i32.const 1
      i32.add
      i32.store offset=8
      local.get 0
      local.get 2
      i32.const -1
      i32.add
      i32.store
      local.get 3
      local.get 1
      i32.store8
    end
    local.get 0
    local.get 0
    i32.load offset=4
    i32.const 1
    i32.add
    i32.store offset=4)
  (func $t__snprintf_parse_flags (type 3) (param i32 i32)
    (local i32)
    local.get 0
    i64.const 0
    i64.store align=4
    local.get 0
    i32.const 16
    i32.add
    i32.const 0
    i32.store
    local.get 0
    i32.const 8
    i32.add
    i64.const 0
    i64.store align=4
    local.get 1
    i32.load
    i32.const -1
    i32.add
    local.set 2
    loop  ;; label = @1
      local.get 0
      local.get 2
      i32.const 1
      i32.add
      local.tee 2
      i32.load8_s
      call $t__snprintf_parse_flag
      i32.const 1
      i32.eq
      br_if 0 (;@1;)
    end
    local.get 1
    local.get 2
    i32.store)
  (func $t__snprintf_parse_width (type 3) (param i32 i32)
    (local i32 i32 i32)
    global.get 0
    i32.const 16
    i32.sub
    local.tee 2
    global.set 0
    local.get 0
    i64.const 0
    i64.store align=4
    block  ;; label = @1
      block  ;; label = @2
        local.get 1
        i32.load
        local.tee 3
        i32.load8_u
        local.tee 4
        i32.const 42
        i32.ne
        br_if 0 (;@2;)
        local.get 0
        i32.const 0
        i32.store
        local.get 1
        local.get 3
        i32.const 1
        i32.add
        i32.store
        br 1 (;@1;)
      end
      block  ;; label = @2
        local.get 4
        i32.const -48
        i32.add
        i32.const 255
        i32.and
        i32.const 10
        i32.lt_u
        br_if 0 (;@2;)
        local.get 0
        i32.const 2
        i32.store
        br 1 (;@1;)
      end
      block  ;; label = @2
        local.get 1
        local.get 2
        i32.const 12
        i32.add
        call $t__snprintf_parse_uint
        i32.eqz
        br_if 0 (;@2;)
        local.get 0
        i32.const 1
        i32.store
        local.get 0
        local.get 2
        i32.load offset=12
        i32.store offset=4
        br 1 (;@1;)
      end
      local.get 0
      i32.const 2
      i32.store
    end
    local.get 2
    i32.const 16
    i32.add
    global.set 0)
  (func $t__snprintf_parse_precision (type 3) (param i32 i32)
    (local i32 i32)
    i32.const 6
    local.set 2
    block  ;; label = @1
      block  ;; label = @2
        local.get 1
        i32.load
        local.tee 3
        i32.load8_u
        i32.const 46
        i32.ne
        br_if 0 (;@2;)
        local.get 1
        local.get 3
        i32.const 1
        i32.add
        i32.store
        local.get 1
        local.get 0
        call $t__snprintf_parse_uint
        br_if 1 (;@1;)
        i32.const 0
        local.set 2
      end
      local.get 0
      local.get 2
      i32.store
    end)
  (func $t__snprintf_add_string (type 3) (param i32 i32)
    (local i32)
    block  ;; label = @1
      local.get 1
      i32.load8_u
      local.tee 2
      i32.eqz
      br_if 0 (;@1;)
      local.get 1
      i32.const 1
      i32.add
      local.set 1
      loop  ;; label = @2
        local.get 0
        local.get 2
        i32.const 24
        i32.shl
        i32.const 24
        i32.shr_s
        call $t__snprintf_add_char
        local.get 1
        i32.load8_u
        local.set 2
        local.get 1
        i32.const 1
        i32.add
        local.set 1
        local.get 2
        br_if 0 (;@2;)
      end
    end)
  (func $t__snprintf_add_int (type 3) (param i32 i32)
    block  ;; label = @1
      local.get 1
      i32.const -1
      i32.gt_s
      br_if 0 (;@1;)
      local.get 0
      i32.const 45
      call $t__snprintf_add_char
      i32.const 0
      local.get 1
      i32.sub
      local.set 1
    end
    local.get 0
    local.get 1
    call $t__snprintf_add_uint)
  (func $t__snprintf_add_uint (type 3) (param i32 i32)
    (local i32 i32 i32 i32)
    block  ;; label = @1
      local.get 1
      i32.eqz
      br_if 0 (;@1;)
      i32.const -1
      local.set 2
      local.get 1
      local.set 3
      loop  ;; label = @2
        local.get 2
        i32.const 1
        i32.add
        local.set 2
        local.get 3
        i32.const 9
        i32.add
        local.set 4
        local.get 3
        i32.const 10
        i32.div_s
        local.set 3
        local.get 4
        i32.const 18
        i32.gt_u
        br_if 0 (;@2;)
      end
      i32.const 0
      local.set 3
      loop  ;; label = @2
        local.get 0
        local.get 1
        local.get 3
        i32.sub
        local.get 2
        call $powi
        local.tee 4
        i32.div_u
        local.tee 5
        i32.const 24
        i32.shl
        i32.const 805306368
        i32.add
        i32.const 24
        i32.shr_s
        call $t__snprintf_add_char
        local.get 5
        local.get 4
        i32.mul
        local.get 3
        i32.add
        local.set 3
        local.get 2
        i32.const -1
        i32.add
        local.tee 2
        i32.const -1
        i32.ne
        br_if 0 (;@2;)
      end
      return
    end
    local.get 0
    i32.const 48
    call $t__snprintf_add_char)
  (func $t__snprintf_add_float (type 4) (param i32 f32 i32)
    (local i32)
    block  ;; label = @1
      block  ;; label = @2
        local.get 1
        f32.abs
        f32.const 0x1p+31 (;=2.14748e+09;)
        f32.lt
        i32.eqz
        br_if 0 (;@2;)
        local.get 1
        i32.trunc_f32_s
        local.set 3
        br 1 (;@1;)
      end
      i32.const -2147483648
      local.set 3
    end
    local.get 0
    local.get 3
    call $t__snprintf_add_int
    block  ;; label = @1
      local.get 2
      i32.eqz
      br_if 0 (;@1;)
      local.get 0
      i32.const 46
      call $t__snprintf_add_char
      block  ;; label = @2
        block  ;; label = @3
          local.get 1
          local.get 3
          f32.convert_i32_s
          f32.sub
          local.get 2
          call $powi
          f32.convert_i32_u
          f32.mul
          local.tee 1
          f32.abs
          f32.const 0x1p+31 (;=2.14748e+09;)
          f32.lt
          i32.eqz
          br_if 0 (;@3;)
          local.get 1
          i32.trunc_f32_s
          local.set 2
          br 1 (;@2;)
        end
        i32.const -2147483648
        local.set 2
      end
      local.get 0
      local.get 2
      local.get 2
      i32.const 31
      i32.shr_s
      local.tee 3
      i32.add
      local.get 3
      i32.xor
      call $t__snprintf_add_int
    end)
  (func $t__snprintf_parse_uint (type 1) (param i32 i32) (result i32)
    (local i32 i32 i32)
    i32.const 0
    local.set 2
    block  ;; label = @1
      local.get 0
      i32.load
      i32.load8_u
      i32.const -48
      i32.add
      i32.const 255
      i32.and
      i32.const 9
      i32.gt_u
      br_if 0 (;@1;)
      i32.const 0
      local.set 3
      block  ;; label = @2
        local.get 0
        i32.load
        local.tee 2
        i32.load8_u
        local.tee 4
        i32.const -48
        i32.add
        i32.const 255
        i32.and
        i32.const 9
        i32.gt_u
        br_if 0 (;@2;)
        local.get 2
        i32.const 1
        i32.add
        local.set 2
        i32.const 0
        local.set 3
        loop  ;; label = @3
          local.get 0
          local.get 2
          i32.store
          local.get 3
          i32.const 10
          i32.mul
          local.get 4
          i32.const 24
          i32.shl
          i32.const 24
          i32.shr_s
          i32.add
          i32.const -48
          i32.add
          local.set 3
          local.get 2
          i32.load8_u
          local.set 4
          local.get 2
          i32.const 1
          i32.add
          local.set 2
          local.get 4
          i32.const -48
          i32.add
          i32.const 255
          i32.and
          i32.const 10
          i32.lt_u
          br_if 0 (;@3;)
        end
      end
      local.get 1
      local.get 3
      i32.store
      i32.const 1
      local.set 2
    end
    local.get 2)
  (func $powi (type 5) (param i32) (result i32)
    (local i32)
    block  ;; label = @1
      local.get 0
      br_if 0 (;@1;)
      i32.const 1
      return
    end
    i32.const 1
    local.set 1
    loop  ;; label = @1
      local.get 1
      i32.const 10
      i32.mul
      local.set 1
      local.get 0
      i32.const -1
      i32.add
      local.tee 0
      br_if 0 (;@1;)
    end
    local.get 1)
  (func $main (type 1) (param i32 i32) (result i32)
    (local i32)
    global.get 0
    i32.const 272
    i32.sub
    local.tee 2
    global.set 0
    i32.const 1024
    call $wasm_print_line
    local.get 2
    call $powi.1
    i32.store
    local.get 2
    i32.const 16
    i32.add
    i32.const 256
    i32.const 1031
    local.get 2
    call $format
    drop
    local.get 2
    i32.const 16
    i32.add
    call $wasm_print_line
    local.get 2
    i32.const 272
    i32.add
    global.set 0
    i32.const 0)
  (func $powi.1 (type 6) (result i32)
    i32.const 256)
  (global (;0;) (mut i32) (i32.const 10098192))
  (global (;1;) i32 (i32.const 10098192))
  (export "main" (func $main))
  (export "__heap_base" (global 1))
  (data (;0;) (i32.const 1024) "Hello!\00%d\0a\00"))
