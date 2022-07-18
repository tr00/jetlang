import Base: ncodeunits, length, codeunit, getindex, checkbounds, iterate

using Libdl

push!(DL_LOAD_PATH, "lib")

struct Csymbol <: AbstractString
    len :: Csize_t
    str :: Cstring

    Csymbol(str :: Cstring, len :: Csize_t = @ccall strlen(str::Cstring)::Csize_t) = cgetsym(str, len)
end

function Csymbol(str :: String)
    ptr = pointer(str)
    len = length(str)

    @assert unsafe_load(ptr, len + 1) == 0x00

    Csymbol(Cstring(ptr), len)
end

@inline cgensym() = @ccall :libsym.gensym()::Csymbol

@inline cgetsym(str :: Cstring, len :: Csize_t) = @ccall :libsym.getsym(str::Cstring, len::Csize_t)::Csymbol

@inline Base.length(sym :: Csymbol) = sym.len
@inline Base.ncodeunits(sym :: Csymbol) = Int(sym.len)

@inline function Base.codeunit(sym :: Csymbol, i :: Integer)
    @boundscheck checkbounds(sym, i)
    
    return Char(unsafe_load(pointer(sym.str), i))
end

Base.@propagate_inbounds Base.getindex(sym :: Csymbol, i :: Integer) = codeunit(sym, i)

@inline function Base.iterate(sym :: Csymbol, i :: Integer = 1)
    if checkbounds(Bool, sym, i)
        return sym[i], i + 1
    else 
        return nothing 
    end
end

