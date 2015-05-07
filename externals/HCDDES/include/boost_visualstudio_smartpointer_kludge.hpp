#ifndef BOOST_KLUDGE
#define BOOST_KLUDGE
namespace boost {
template<> class intrusive_ptr< void >
{
private:

    typedef intrusive_ptr this_type;

public:

    typedef void element_type;

    intrusive_ptr(): p_(0)
    {
    }

    intrusive_ptr(void * p, bool add_ref = true): p_(p)
    {
//        if(p_ != 0 && add_ref) intrusive_ptr_add_ref(p_);
    }

#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)

    template<class U>
#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )

    intrusive_ptr( intrusive_ptr<U> const & rhs, typename detail::sp_enable_if_convertible<U,void>::type = detail::sp_empty() )

#else

    intrusive_ptr( intrusive_ptr<U> const & rhs )

#endif
    : p_( rhs.get() )
    {
//        if( p_ != 0 ) intrusive_ptr_add_ref( p_ );
    }

#endif

    intrusive_ptr(intrusive_ptr const & rhs): p_(rhs.p_)
    {
//        if(p_ != 0) intrusive_ptr_add_ref(p_);
    }

    ~intrusive_ptr()
    {
//        if(p_ != 0) intrusive_ptr_release(p_);
    }

#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)

    template<class U> intrusive_ptr & operator=(intrusive_ptr<U> const & rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

#endif

    intrusive_ptr & operator=(intrusive_ptr const & rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    intrusive_ptr & operator=(void * rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    void reset()
    {
        this_type().swap( *this );
    }

    void reset( void * rhs )
    {
        this_type( rhs ).swap( *this );
    }

    void * get() const
    {
        return p_;
    }

//    void & operator*() const
//    {
//        BOOST_ASSERT( p_ != 0 );
//        return *p_;
//    }

    void * operator->() const
    {
        BOOST_ASSERT( p_ != 0 );
        return p_;
    }

#if defined(__SUNPRO_CC) && BOOST_WORKAROUND(__SUNPRO_CC, <= 0x530)

    operator bool () const
    {
        return p_ != 0;
    }

#elif defined(__MWERKS__) && BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3003))
    typedef void * (this_type::*unspecified_bool_type)() const;
    
    operator unspecified_bool_type() const // never throws
    {
        return p_ == 0? 0: &this_type::get;
    }

#else 

    typedef void * this_type::*unspecified_bool_type;

    operator unspecified_bool_type () const
    {
        return p_ == 0? 0: &this_type::p_;
    }

#endif

    // operator! is a Borland-specific workaround
    bool operator! () const
    {
        return p_ == 0;
    }

    void swap(intrusive_ptr & rhs)
    {
        void * tmp = p_;
        p_ = rhs.p_;
        rhs.p_ = tmp;
    }

private:

    void * p_;
};
}
#endif