#ifndef FLAGS_H
#define FLAGS_H

template <class Flag>
class Flags
{
public:
    Flags() { }
    Flags(int flagsOn) { SetOn(flagsOn); }
    Flags(Flag flagsOn) : Flags( static_cast<int>(flagsOn) ) { }

    void SetOn(int f)  { m_flags = m_flags | f; }
    void SetOn(Flag f) { SetOn(static_cast<int>(f)); }

    void SetOff(int f)  { m_flags = m_flags & ~f; }
    void SetOff(Flag f) { SetOff(static_cast<int>(f)); }

    bool IsOn(int f)  const { return (m_flags & f) > 0; }
    bool IsOn(Flag f) const { IsOn(static_cast<int>(f)); }

    inline Flags operator|(Flag f)
    {
        return Flags(static_cast<Flag>(m_flags | f));
    }

    inline Flags operator~()
    {
        return Flags(static_cast<Flags>(~m_flags));
    }

    inline Flags operator&(Flag f)
    {
        return Flags(static_cast<Flag>(m_flags & f));
    }

    inline Flags operator^(Flag f)
    {
        return Flags(static_cast<Flag>(m_flags ^ f));
    }

    int ToInteger() const
    {
        return static_cast<int>(m_flags);
    }

private:
    int m_flags = 0;
};

#define CREATE_FLAGS(FlagsName, FlagType) typedef Flags<FlagType> FlagsName

#endif // FLAGS_H
