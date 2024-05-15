#ifndef __INCLUDED_sram_64_32_1w1r_H__
#define __INCLUDED_sram_64_32_1w1r_H__

// SystemC memory model of sram_64_32_1w1r
// Generated by Catapult memory generator
// Generated by:   bkaether
// Generated date: Tue May 14 20:44:10 2024

// The following classes have been defined:
//     class sram_64_32_1w1r a 0 delay "structural" model of the RAM

//     class sram_64_32_1w1r_model<TLM/SYN>::mem<T,size,TLM/SYN>
//         A memory model that can be instanced and used for simulation and synthesis

//     class sram_64_32_1w1r_model<TLM/SYN>::port_0_port<T,size,TLM/SYN>
//         a model of an external port with write capability

//     class sram_64_32_1w1r_model<TLM/SYN>::port_1_port<T,size,TLM/SYN>
//         a model of an external port with read capability

#include <ccs_types.h>
#ifndef NO_ASSERTS
#include <assert.h>
#endif

#pragma map_to_operator sram_64_32_1w1r
template <
  int ram_id>
class sram_64_32_1w1r : public sc_module
{
public:
  sc_in< bool >   clk0;
  sc_in< bool >   csb0;
  sc_in< bool >   web0;
  sc_in< sc_lv<6> >   addr0;
  sc_in< sc_lv<32> >   din0;
  sc_in< bool >   clk1;
  sc_in< bool >   csb1;
  sc_in< sc_lv<6> >   addr1;
  sc_out< sc_lv<32> >   dout1;

#if !defined(__SYNTHESIS__)
  sc_signal<sc_lv<32> > mem[64];
#endif

  SC_CTOR(sram_64_32_1w1r) :
    clk0("clk0")
    ,csb0("csb0")
    ,web0("web0")
    ,addr0("addr0")
    ,din0("din0")
    ,clk1("clk1")
    ,csb1("csb1")
    ,addr1("addr1")
    ,dout1("dout1")
  {
    SC_METHOD(seq_proc_1);
    dont_initialize();
    sensitive << clk0.pos();
    SC_METHOD(seq_proc_2);
    dont_initialize();
    sensitive << clk1.pos();
  }

  void seq_proc_1() {
#if !defined(__SYNTHESIS__)
    int address;
    // write X
    if ((csb0==0) && (csb1==0) && (web0==0)) {
      vector_to_type( addr0.read(), false, &address );
#if !defined(NO_ASSERTS) && !defined(__SYNTHESIS__)
      assert( address>=0 && address <64 );
#endif
      mem[address] = din0.read();
    }

    // read
    // write
    if ((csb0==0) && (csb1==0) && (web0==0)) {
      vector_to_type( addr0.read(), false, &address );
#if !defined(NO_ASSERTS) && !defined(__SYNTHESIS__)
      assert( address>=0 && address <64 );
#endif
      mem[address] = din0.read();
    }

#endif
  }

  void seq_proc_2() {
#if !defined(__SYNTHESIS__)
    int address;
    // write X
    // read
    if ( (csb0==0) && (csb1==0) ) {
      vector_to_type( addr1.read(), false, &address );
#if !defined(NO_ASSERTS) && !defined(__SYNTHESIS__)
      assert( address>=0 && address <64 );
#endif
      dout1.write( mem[address] );
    }
    else dout1.write( sc_lv< 32>() );

    // write
#endif
  }

#if !defined(__SYNTHESIS__)
  // NOTE: this method is intended for use by testbenches only. It is not synthesizable!
  sc_lv< 32> tb_read( int address ) const {
#ifndef NO_ASSERTS
    assert( address>=0 && address < 64);
#endif
    return mem[address];
  }

  // NOTE: this method is intended for use by testbenches only. It is not synthesizable!
  void tb_write( int address, sc_lv<32> data ) {
#ifndef NO_ASSERTS
    assert( address>=0 && address < 64);
#endif
    mem[address] = data;
  }

#endif
};

template <abstraction_t source_abstraction = AUTO>
class sram_64_32_1w1r_model {
public:
#if defined(__SYNTHESIS__)
  template <class T, unsigned int size, abstraction_t impl_abstraction = SYN> class mem{};
private:
  template <class T, unsigned int size, abstraction_t impl_abstraction = SYN> class port_0_local;
  template <class T, unsigned int size, abstraction_t impl_abstraction = SYN> class port_1_local;
public:
  template <class T, unsigned int size, abstraction_t impl_abstraction = SYN> class port_0_port;
  template <class T, unsigned int size, abstraction_t impl_abstraction = SYN> class port_1_port;

#elif defined (CCS_DUT_CYCLE) || defined (CCS_DUT_RTL)
  template <class T, unsigned int size, abstraction_t impl_abstraction = (source_abstraction==AUTO) ? SYN : source_abstraction>
      class mem{};
private:
  template <class T, unsigned int size, abstraction_t impl_abstraction = (source_abstraction==AUTO) ? SYN : source_abstraction>
      class port_0_local;
  template <class T, unsigned int size, abstraction_t impl_abstraction = (source_abstraction==AUTO) ? SYN : source_abstraction>
      class port_1_local;
public:
  template <class T, unsigned int size, abstraction_t impl_abstraction = (source_abstraction==AUTO) ? SYN : source_abstraction>
      class port_0_port;
  template <class T, unsigned int size, abstraction_t impl_abstraction = (source_abstraction==AUTO) ? SYN : source_abstraction>
      class port_1_port;

#else
  template <class T, unsigned int size, abstraction_t impl_abstraction = (source_abstraction==AUTO) ? P2P_DEFAULT_VIEW
      : source_abstraction> class mem{};
private:
  template <class T, unsigned int size, abstraction_t impl_abstraction = (source_abstraction==AUTO) ? P2P_DEFAULT_VIEW
      : source_abstraction> class port_0_local;
  template <class T, unsigned int size, abstraction_t impl_abstraction = (source_abstraction==AUTO) ? P2P_DEFAULT_VIEW
      : source_abstraction> class port_1_local;
public:
  template <class T, unsigned int size, abstraction_t impl_abstraction = (source_abstraction==AUTO) ? P2P_DEFAULT_VIEW
      : source_abstraction> class port_0_port;
  template <class T, unsigned int size, abstraction_t impl_abstraction = (source_abstraction==AUTO) ? P2P_DEFAULT_VIEW
      : source_abstraction> class port_1_port;

#endif

  // ***********************************************
  // TLM TLM TLM TLM TLM TLM TLM TLM TLM TLM TLM TLM
  //    This code is only for TLM simulation only
  // ***********************************************

  template <class T, unsigned int size>
  class mem <T,size,TLM>
  {
    enum {
      addr_width = nbits<size-1>::val,
      signedtype = mc_typedef_T_traits<T>::issigned,
      data_width = mc_typedef_T_traits<T>::bitwidth
    };
  public:
    void write (ac_int<addr_width,false> addr, T data=0) {
      port_0_port_instance.reset_chk.test();
#ifndef NO_ASSERTS
      assert( addr>=0 && addr<size );
#endif
      array[addr] = data;
    };

    T read ( ac_int<addr_width,false> addr ) {
#ifndef NO_ASSERTS
      assert( addr>=0 && addr<size );
#endif
      port_1_port_instance.reset_chk.test();
      return array[addr];
    }

    T &operator [] (int index) {
#ifndef NO_ASSERTS
      assert( index>=0 && index<size );
#endif
      return array[index];
    }

    const T &operator [] (int index) const {
#ifndef NO_ASSERTS
      assert( index>=0 && index<size );
#endif
      return array[index];
    }

    // NOTE: this method is intended for use by testbenches only. It is not synthesizable!
    T tb_read( int address ) const {
#ifndef NO_ASSERTS
      assert( address>=0 && address <size );
#endif
      return array[address];
    }

    // NOTE: this method is intended for use by testbenches only. It is not synthesizable!
    void tb_write( int address, T data ) {
#ifndef NO_ASSERTS
      assert( address>=0 && address <size );
#endif
      array[address] = data;
    }

  public:
    mem(const char* name)
      : port_0_port_instance(*this, ccs_concat(name,"port_0"))
      , port_1_port_instance(*this, ccs_concat(name,"port_1"))
      {}

    void reset_read() {
      port_1_port_instance.reset();
    }

    void reset_write() {
      port_0_port_instance.reset();
    }

    void clk0(sc_in<bool> &clk0_arg) {
      port_0_port_instance.clock_ok();
    }
    void clk0(sc_signal<bool> &clk0_arg) {
      port_0_port_instance.clock_ok();
    }

    void clk1(sc_in<bool> &clk1_arg) {
      port_1_port_instance.clock_ok();
    }
    void clk1(sc_signal<bool> &clk1_arg) {
      port_1_port_instance.clock_ok();
    }

    port_0_local<T,size,TLM> port_0_port_instance;
    port_1_local<T,size,TLM> port_1_port_instance;
  private:
    T array[size];
    friend class port_0_local<T,size,TLM>;
    friend class port_1_local<T,size,TLM>;
  };

private:
  template <class T, unsigned int size>
  class port_0_local<T,size,TLM>
  {
    enum {
      addr_width = nbits<size-1>::val,
      signedtype = mc_typedef_T_traits<T>::issigned,
      data_width = mc_typedef_T_traits<T>::bitwidth
    };
    p2p_checker reset_chk;
    p2p_checker clock_chk;
    void clock_ok() {clock_chk.ok();}

  public:
    port_0_local (mem<T,size,TLM> &my_parent, const char *name)
      : reset_chk( name, "call reset()", "access this memory")
      , clock_chk( name, "bind the clock port", "access this memory")
      , parent(my_parent)
      {}

    void reset() {reset_chk.ok();}

    T &operator [] (int index) {
      reset_chk.test();
      clock_chk.test();
      return parent.array[index];
    }

    const T &operator [] (int index) const {
      reset_chk.test();
      clock_chk.test();
#ifndef NO_ASSERTS
      assert( index>=0 && index<size );
#endif
      return parent.array[index];
    }

    void write (ac_int<addr_width,false> addr, T data=0) {
      reset_chk.test();
      clock_chk.test();
#ifndef NO_ASSERTS
      assert( addr>=0 && addr<size );
#endif
      parent.array[addr] = data;
    };

  private:
    mem<T,size,TLM> &parent;
    friend class mem<T,size,TLM>;
  };

  template <class T, unsigned int size>
  class port_1_local<T,size,TLM>
  {
    enum {
      addr_width = nbits<size-1>::val,
      signedtype = mc_typedef_T_traits<T>::issigned,
      data_width = mc_typedef_T_traits<T>::bitwidth
    };
    p2p_checker reset_chk;
    p2p_checker clock_chk;
    void clock_ok() {clock_chk.ok();}

  public:
    port_1_local (mem<T,size,TLM> &my_parent, const char *name)
      : reset_chk( name, "call reset()", "access this memory")
      , clock_chk( name, "bind the clock port", "access this memory")
      , parent(my_parent)
      {}

    void reset() {reset_chk.ok();}

    const T &operator [] (int index) const {
      reset_chk.test();
      clock_chk.test();
#ifndef NO_ASSERTS
      assert( index>=0 && index<size );
#endif
      return parent.array[index];
    }

    T read ( ac_int<addr_width,false> addr ) {
      reset_chk.test();
      clock_chk.test();
#ifndef NO_ASSERTS
      assert( addr>=0 && addr<size );
#endif
      return parent.read(addr);
    }

  private:
    mem<T,size,TLM> &parent;
    friend class mem<T,size,TLM>;
  };

public:
  template <class T, unsigned int size>
  class port_0_port<T,size,TLM>
  {
    enum {
      addr_width = nbits<size-1>::val,
      signedtype = mc_typedef_T_traits<T>::issigned,
      data_width = mc_typedef_T_traits<T>::bitwidth
    };
    p2p_checker reset_chk;

  public:
    port_0_port(const char *name)
      : reset_chk(name, "call reset()", "access this port")
      , chan_ptr(0)
      , port_ptr(0)
      {}

    void write(unsigned int addr, T data ) {
      reset_chk.test();
      if ( bound_to_port )
        port_ptr->write(addr,data);
      else
        chan_ptr->write(addr,data);
    }

    T &operator [] (int index) {
      reset_chk.test();
      return (bound_to_port? (*port_ptr)[index] : (*chan_ptr)[index]);
    }

    void reset() {reset_chk.ok();}

    void operator() (port_0_local<T,size,TLM> &c) {
      chan_ptr = &c;
      c.reset(); // Supress reset error on bound memory
      bound_to_port = false;
    }

    void operator() (port_0_port<T,size,TLM> &c) {
      port_ptr = &c;
      c.reset(); // Supress reset error on bound port
      bound_to_port = true;
    }

    void operator() (mem<T,size,TLM> &m) {
      chan_ptr = &m.port_0_port_instance;
      chan_ptr->reset(); // Supress reset error on bound memory
      bound_to_port = false;
    }

  private:
    bool bound_to_port;
    port_0_local<T,size,TLM> *chan_ptr;
    port_0_port<T,size,TLM> *port_ptr;
  };

  template <class T, unsigned int size>
  class port_1_port<T,size,TLM>
  {
    enum {
      addr_width = nbits<size-1>::val,
      signedtype = mc_typedef_T_traits<T>::issigned,
      data_width = mc_typedef_T_traits<T>::bitwidth
    };
    p2p_checker reset_chk;

  public:
    port_1_port(const char *name)
      : reset_chk(name, "call reset()", "access this port")
      , chan_ptr(0)
      , port_ptr(0)
      {}

    T read(unsigned int addr) {
      reset_chk.test();
      if ( bound_to_port )
        return port_ptr->read(addr);
      else
        return chan_ptr->read(addr);
    }

    const T &operator [] (int index) {
      reset_chk.test();
      return (bound_to_port? (*port_ptr)[index] : (*chan_ptr)[index]);
    }

    void reset() {reset_chk.ok();}

    void operator() (port_1_local<T,size,TLM> &c) {
      chan_ptr = &c;
      c.reset(); // Supress reset error on bound memory
      bound_to_port = false;
    }

    void operator() (port_1_port<T,size,TLM> &c) {
      port_ptr = &c;
      c.reset(); // Supress reset error on bound port
      bound_to_port = true;
    }

    void operator() (mem<T,size,TLM> &m) {
      chan_ptr = &m.port_1_port_instance;
      chan_ptr->reset(); // Supress reset error on bound memory
      bound_to_port = false;
    }

  private:
    bool bound_to_port;
    port_1_local<T,size,TLM> *chan_ptr;
    port_1_port<T,size,TLM> *port_ptr;
  };

  // ***********************************************
  // SYN SYN SYN SYN SYN SYN SYN SYN SYN SYN SYN SYN
  // Synthesis code
  // ***********************************************

private:
  template <class T, unsigned int size>
  class port_0_local<T,size,SYN>
  {
    enum {
      addr_width = nbits<size-1>::val,
      signedtype = mc_typedef_T_traits<T>::issigned,
      data_width = mc_typedef_T_traits<T>::bitwidth
    };
    p2p_checker reset_chk;
    p2p_checker clock_chk;
    class mem_ref
    {
    public:
      mem_ref( port_0_local<T,size,SYN> &my_parent, ac_int<addr_width,false> addr)
      : address(addr), parent(my_parent) {}

      inline mem_ref operator = ( T val ) {
        parent.write(address,val);
        return *this;
      }

      inline mem_ref operator = ( const mem_ref &val ) { return operator = ((T)val); }

    private:
      ac_int<addr_width,false> address;
      port_0_local<T,size,SYN> &parent;
    };

    void clock_ok() {clock_chk.ok();}

  public:
    port_0_local( const char *name)
      : reset_chk( name, "call reset()", "access this memory")
      , clock_chk( name, "bind the clock port", "access this memory")
      , web0(ccs_concat(name,"web0"))
      , addr0(ccs_concat(name,"addr0"))
      , din0(ccs_concat(name,"din0"))
      , csb0(ccs_concat(name,"csb0"))
      , csb1(ccs_concat(name,"csb1"))
      {}

    #pragma remove_out_reg
    #pragma design modulario
    void write (ac_int<addr_width,false> addr, T data=0) {
      reset_chk.test();
      clock_chk.test();
      csb0.write(0);
      csb1.write(0);
      sc_lv<data_width> temp_data;
      type_to_vector(data, (signedtype?true:false), temp_data);
      sc_lv<addr_width> temp_addr;
      type_to_vector(addr, (signedtype?true:false), temp_addr);
      web0.write(0);
      addr0.write( temp_addr );
      din0.write( temp_data );
      #pragma unroll
      for (int lcnt=0; lcnt<1; lcnt++) wait();
#if defined(CALYPTO_SC)
      wait(); // Account for unreg outputs for SLEC
#elif !defined(__SYNTHESIS__)
      wait(0.3, SC_NS);  // Account for unreg outputs in simulation
#endif
      web0.write(1);
      csb0.write(1);
      csb1.write(1);
#if !defined(CALYPTO_SC)
      // Return outputs to don't care to save area
      sc_lv<data_width> dc;
      din0.write( dc );
      addr0.write( dc );
#endif
    }

    mem_ref operator [] (int index) {
      mem_ref write_ref(*this, index);
      return write_ref;
    }

    void reset() {
      web0.write(1);
      csb0.write(1);
      csb1.write(1);
      addr0.write(sc_dt::Log_0);
      din0.write(sc_dt::Log_0);
      reset_chk.ok();
    }

    sc_signal< bool > web0;
    sc_signal< sc_lv<addr_width> > addr0;
    sc_signal< sc_lv<data_width> > din0;
    sc_signal< bool >  csb0;
    sc_signal< bool >  csb1;
  private:
    friend class mem<T,size,SYN>;
  };

  template <class T, unsigned int size>
  class port_1_local<T,size,SYN>
  {
    enum {
      addr_width = nbits<size-1>::val,
      signedtype = mc_typedef_T_traits<T>::issigned,
      data_width = mc_typedef_T_traits<T>::bitwidth
    };
    p2p_checker reset_chk;
    p2p_checker clock_chk;
    class mem_ref
    {
    public:
      mem_ref( port_1_local<T,size,SYN> &my_parent, ac_int<addr_width,false> addr)
      : address(addr), parent(my_parent) {}

      operator T() const { return parent.read(address); }

      inline mem_ref operator = ( const mem_ref &val ) { return operator = ((T)val); }

    private:
      ac_int<addr_width,false> address;
      port_1_local<T,size,SYN> &parent;
    };

    void clock_ok() {clock_chk.ok();}

  public:
    port_1_local( const char *name)
      : reset_chk( name, "call reset()", "access this memory")
      , clock_chk( name, "bind the clock port", "access this memory")
      , addr1(ccs_concat(name,"addr1"))
      , dout1(ccs_concat(name,"dout1"))
      , csb0(ccs_concat(name,"csb0"))
      , csb1(ccs_concat(name,"csb1"))
      {}

    #pragma remove_out_reg
    #pragma design modulario
    T read (ac_int<addr_width,false> addr) {
      reset_chk.test();
      clock_chk.test();
      csb0.write(0);
      csb1.write(0);
      sc_lv<addr_width> temp_addr;
      type_to_vector(addr, (signedtype?true:false), temp_addr);
      addr1.write( temp_addr );
      #pragma unroll
      for (int lcnt=0; lcnt<1; lcnt++) wait();
#if defined(CALYPTO_SC)
      wait(); // Account for unreg outputs for SLEC
#elif !defined(__SYNTHESIS__)
      wait(0.3, SC_NS);  // Account for unreg outputs in simulation
#endif
      csb0.write(1);
      csb1.write(1);
#if !defined(CALYPTO_SC)
      // Return outputs to don't care to save area
      sc_lv<data_width> dc;
      addr1.write( dc );
#endif
      T return_data;
      vector_to_type(dout1.read(), (signedtype?true:false), &return_data);
      return return_data;
    }

    mem_ref operator [] (int index) {
      mem_ref write_ref(*this, index);
      return write_ref;
    }

    void reset() {
      csb0.write(1);
      csb1.write(1);
      addr1.write(sc_dt::Log_0);
      reset_chk.ok();
    }

    sc_signal< sc_lv<addr_width> > addr1;
    sc_signal< sc_lv<data_width> > dout1;
    sc_signal< bool >  csb0;
    sc_signal< bool >  csb1;
  private:
    friend class mem<T,size,SYN>;
  };

public:
  template <class T, unsigned int size>
  class mem <T,size,SYN>
  {
    enum {
      addr_width = nbits<size-1>::val,
      signedtype = mc_typedef_T_traits<T>::issigned,
      data_width = mc_typedef_T_traits<T>::bitwidth
    };
    class mem_ref
    {
    public:
      mem_ref( mem<T,size,SYN> &my_parent, ac_int<addr_width,false> addr)
      : address(addr), parent(my_parent) {}

      operator T() const { return parent.read(address); }

      inline mem_ref operator = ( T val ) {
        parent.write(address,val);
        return *this;
      }

      inline mem_ref operator = ( const mem_ref &val ) { return operator = ((T)val); }

    private:
      ac_int<addr_width,false> address;
      mem<T,size,SYN> &parent;
    };

    #pragma ungroup
    struct gen_csb0 : public sc_module
    {
      sc_in<bool> port_0;
      sc_in<bool> port_1;
      sc_out<bool> output;

      SC_CTOR(gen_csb0) :
        port_0("port_0")
        ,port_1("port_1")
        ,output("output")
      {
        SC_METHOD(do_it);
        dont_initialize();;
        sensitive << port_0 << port_1;
      }

      void do_it()
      {
        output.write( port_0 & port_1 );
      }
    };

    #pragma ungroup
    struct gen_csb1 : public sc_module
    {
      sc_in<bool> port_0;
      sc_in<bool> port_1;
      sc_out<bool> output;

      SC_CTOR(gen_csb1) :
        port_0("port_0")
        ,port_1("port_1")
        ,output("output")
      {
        SC_METHOD(do_it);
        dont_initialize();;
        sensitive << port_0 << port_1;
      }

      void do_it()
      {
        output.write( port_0 & port_1 );
      }
    };

  public:
    mem ( const char *name)
      : RAM(ccs_concat(name, "RAM"))
      , port_0_port_instance(ccs_concat(name,"port_0"))
      , port_1_port_instance(ccs_concat(name,"port_1"))
      , gen_csb0_instance(ccs_concat(name,"gen_csb0"))
      , gen_csb1_instance(ccs_concat(name,"gen_csb1"))
    {
      RAM.web0(port_0_port_instance.web0);
      RAM.addr0(port_0_port_instance.addr0);
      RAM.din0(port_0_port_instance.din0);
      RAM.addr1(port_1_port_instance.addr1);
      RAM.dout1(port_1_port_instance.dout1);
      RAM.csb0(csb0);
      gen_csb0_instance.port_0(port_0_port_instance.csb0);
      gen_csb0_instance.port_1(port_1_port_instance.csb0);
      gen_csb0_instance.output(csb0);
      RAM.csb1(csb1);
      gen_csb1_instance.port_0(port_0_port_instance.csb1);
      gen_csb1_instance.port_1(port_1_port_instance.csb1);
      gen_csb1_instance.output(csb1);
    }

    port_0_local<T,size,SYN> port_0_port_instance;
    port_1_local<T,size,SYN> port_1_port_instance;
    gen_csb0 gen_csb0_instance;
    gen_csb1 gen_csb1_instance;

    void clk0(sc_in<bool> &clk0_arg) {
      RAM.clk0(clk0_arg);
      port_0_port_instance.clock_ok();
    }
    void clk0(sc_signal<bool> &clk0_arg) {
      RAM.clk0(clk0_arg);
      port_0_port_instance.clock_ok();
    }

    void clk1(sc_in<bool> &clk1_arg) {
      RAM.clk1(clk1_arg);
      port_1_port_instance.clock_ok();
    }
    void clk1(sc_signal<bool> &clk1_arg) {
      RAM.clk1(clk1_arg);
      port_1_port_instance.clock_ok();
    }


    T read ( ac_int<addr_width,false> addr ) {
      return port_1_port_instance.read( addr );
    }

    void write (ac_int<addr_width,false> addr, T data=0) {
      port_0_port_instance.write( addr, data );
    }

    mem_ref operator [] (int index) {
      mem_ref write_ref(*this, index);
      return write_ref;
    }

    void reset_read() {
      port_1_port_instance.reset();
    }

    void reset_write() {
      port_0_port_instance.reset();
    }

    // NOTE: this method is intended for use by testbenches only. It is not synthesizable!
    T tb_read(int address) const {
#ifndef NO_ASSERTS
      assert( address>=0 && address<size );
#endif
      T return_data;
      vector_to_type( RAM.tb_read(address) , (signedtype?true:false), &return_data);
      return return_data;
    }

    // NOTE: this method is intended for use by testbenches only. It is not synthesizable!
    void tb_write( int address, T val) {
#ifndef NO_ASSERTS
      assert( address>=0 && address<size );
#endif
      sc_lv<data_width> temp_data;
      type_to_vector(val, (signedtype?true:false), temp_data);
      RAM.tb_write(address, temp_data);
    }

  private:
    sram_64_32_1w1r<0> RAM;
    sc_signal< bool >  csb0;
    sc_signal< bool >  csb1;
  };

  template <class T, unsigned int size>
  class port_0_port<T,size,SYN>
  {
    enum {
      addr_width = nbits<size-1>::val,
      signedtype = mc_typedef_T_traits<T>::issigned,
      data_width = mc_typedef_T_traits<T>::bitwidth
    };
    p2p_checker reset_chk;

    class mem_ref
    {
    public:
      mem_ref( port_0_port<T,size,SYN> &my_parent, ac_int<addr_width,false> addr)
      : address(addr), parent(my_parent) {}

      inline mem_ref operator = ( T val ) {
        parent.write(address,val);
        return *this;
      }

      inline mem_ref operator = ( const mem_ref &val ) { return operator = ((T)val); }

    private:
      ac_int<addr_width,false> address;
      port_0_port<T,size,SYN> &parent;
    };

  public:
    port_0_port( const char *name)
      : reset_chk( name, "call reset()", "access this memory")
      , web0(ccs_concat(name,"web0"))
      , addr0(ccs_concat(name,"addr0"))
      , din0(ccs_concat(name,"din0"))
      , csb0(ccs_concat(name,"csb0"))
      , csb1(ccs_concat(name,"csb1"))
      {}

    #pragma remove_out_reg
    #pragma design modulario
    void write (ac_int<addr_width,false> addr, T data=0) {
      reset_chk.test();
      csb0.write(0);
      csb1.write(0);
      sc_lv<data_width> temp_data;
      type_to_vector(data, (signedtype?true:false), temp_data);
      sc_lv<addr_width> temp_addr;
      type_to_vector(addr, (signedtype?true:false), temp_addr);
      web0.write(0);
      addr0.write( temp_addr );
      din0.write( temp_data );
      #pragma unroll
      for (int lcnt=0; lcnt<1; lcnt++) wait();
#if defined(CALYPTO_SC)
      wait(); // Account for unreg outputs for SLEC
#elif !defined(__SYNTHESIS__)
      wait(0.3, SC_NS);  // Account for unreg outputs in simulation
#endif
      web0.write(1);
      csb0.write(1);
      csb1.write(1);
#if !defined(CALYPTO_SC)
      // Return outputs to don't care to save area
      sc_lv<data_width> dc;
      din0.write( dc );
      addr0.write( dc );
#endif
    }

    void reset() {
      web0.write(1);
      csb0.write(1);
      csb1.write(1);
      addr0.write(sc_dt::Log_0);
      din0.write(sc_dt::Log_0);
      reset_chk.ok();
    }

    sc_out< bool > web0;
    sc_out< sc_lv<addr_width> > addr0;
    sc_out< sc_lv<data_width> > din0;
    sc_out< bool >  csb0;
    sc_out< bool >  csb1;

    mem_ref operator [] (int index) {
      mem_ref write_ref(*this, index);
      return write_ref;
    }

    template <class C> void bind( C &c ) {
      web0(c.web0);
      addr0(c.addr0);
      din0(c.din0);
      csb0(c.csb0);
      csb1(c.csb1);
    }

    template <class C> void operator() (C& c) {
      bind(c);
    }

    void operator() (mem<T,size,SYN> &m) {
      web0(m.port_0_port_instance.web0);
      addr0(m.port_0_port_instance.addr0);
      din0(m.port_0_port_instance.din0);
      csb0(m.port_0_port_instance.csb0);
      csb1(m.port_0_port_instance.csb1);
    }

  };


  template <class T, unsigned int size>
  class port_1_port<T,size,SYN>
  {
    enum {
      addr_width = nbits<size-1>::val,
      signedtype = mc_typedef_T_traits<T>::issigned,
      data_width = mc_typedef_T_traits<T>::bitwidth
    };
    p2p_checker reset_chk;

    class mem_ref
    {
    public:
      mem_ref( port_1_port<T,size,SYN> &my_parent, ac_int<addr_width,false> addr)
      : address(addr), parent(my_parent) {}

      operator T() const { return parent.read(address); }

      inline mem_ref operator = ( const mem_ref &val ) { return operator = ((T)val); }

    private:
      ac_int<addr_width,false> address;
      port_1_port<T,size,SYN> &parent;
    };

  public:
    port_1_port( const char *name)
      : reset_chk( name, "call reset()", "access this memory")
      , addr1(ccs_concat(name,"addr1"))
      , dout1(ccs_concat(name,"dout1"))
      , csb0(ccs_concat(name,"csb0"))
      , csb1(ccs_concat(name,"csb1"))
      {}

    #pragma remove_out_reg
    #pragma design modulario
    T read (ac_int<addr_width,false> addr) {
      reset_chk.test();
      csb0.write(0);
      csb1.write(0);
      sc_lv<addr_width> temp_addr;
      type_to_vector(addr, (signedtype?true:false), temp_addr);
      addr1.write( temp_addr );
      #pragma unroll
      for (int lcnt=0; lcnt<1; lcnt++) wait();
#if defined(CALYPTO_SC)
      wait(); // Account for unreg outputs for SLEC
#elif !defined(__SYNTHESIS__)
      wait(0.3, SC_NS);  // Account for unreg outputs in simulation
#endif
      csb0.write(1);
      csb1.write(1);
#if !defined(CALYPTO_SC)
      // Return outputs to don't care to save area
      sc_lv<data_width> dc;
      addr1.write( dc );
#endif
      T return_data;
      vector_to_type(dout1.read(), (signedtype?true:false), &return_data);
      return return_data;
    }

    void reset() {
      csb0.write(1);
      csb1.write(1);
      addr1.write(sc_dt::Log_0);
      reset_chk.ok();
    }

    sc_out< sc_lv<addr_width> > addr1;
    sc_in< sc_lv<data_width> > dout1;
    sc_out< bool >  csb0;
    sc_out< bool >  csb1;

    mem_ref operator [] (int index) {
      mem_ref write_ref(*this, index);
      return write_ref;
    }

    template <class C> void bind( C &c ) {
      addr1(c.addr1);
      dout1(c.dout1);
      csb0(c.csb0);
      csb1(c.csb1);
    }

    template <class C> void operator() (C& c) {
      bind(c);
    }

    void operator() (mem<T,size,SYN> &m) {
      addr1(m.port_1_port_instance.addr1);
      dout1(m.port_1_port_instance.dout1);
      csb0(m.port_1_port_instance.csb0);
      csb1(m.port_1_port_instance.csb1);
    }

  };

};
#endif // ifndef __INCLUDED_sram_64_32_1w1r_H__


