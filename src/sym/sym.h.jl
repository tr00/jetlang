import Base: ncodeunits, length, codeunit, getindex, iterate, string, show

using Libdl

push!(DL_LOAD_PATH, "lib")

"""
typedef struct
{
    int len;
    char data[];
} symbol_t;
"""
struct Csymbol <: AbstractString
    ptr :: Ptr{Cvoid}

    Csymbol(str :: Cstring, len :: Cint) = cgetsym(str, len)
end

function Csymbol(str :: String)
    ptr = pointer(str)
    len = length(str)

    @assert unsafe_load(ptr, len + 1) == 0x00

    return Csymbol(Cstring(ptr), Cint(len))
end

"""
symbol_t *gensym();
"""
@inline function cgensym() 
    @ccall :libsym.gensym()::Csymbol
end

"""
symbol_t *getsym(const char *str, size_t len);
"""
@inline function cgetsym(str :: Cstring, len :: Cint) 
    @ccall :libsym.getsym(str::Cstring, len::Cint)::Csymbol
end

@inline function Base.ncodeunits(sym :: Csymbol)
    ptr = Ptr{Cint}(sym.ptr)
    len = unsafe_load(ptr)

    return Int(len)
end

@inline function Base.codeunit(sym :: Csymbol, i :: Integer)
    @boundscheck checkbounds(sym, i)

    ptr = Ptr{Cchar}(sym.ptr) + Core.sizeof(Cint)
    ccu = unsafe_load(ptr, i)

    return Char(ccu)
end

@inline function Base.length(sym :: Csymbol)
    ncodeunits(sym)
end

Base.@propagate_inbounds function Base.getindex(sym :: Csymbol, i :: Integer)
    codeunit(sym, i)
end

@inline function Base.iterate(sym :: Csymbol, i :: Integer = 1)
    if checkbounds(Bool, sym, i)
        return sym[i], i + 1
    else 
        return nothing 
    end
end

function Base.string(sym :: Csymbol)
    ptr = Ptr{UInt8}(sym.ptr + Core.sizeof(Cint))
    len = length(sym)
    str = unsafe_string(ptr, len)

    return str
end

function Base.show(io :: IO, sym :: Csymbol)
    print(io, string(sym))
end

@ccall :libsym.judy_init()::Cvoid