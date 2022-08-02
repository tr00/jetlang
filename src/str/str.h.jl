using Base: ncodeunits, length, codeunit

struct Cnstring <: AbstractString
    str :: String
end

Base.length(cns :: Cnstring) = length(cns.str)
Base.ncodeunits(cns :: Cnstring) = ncodeunits(cns.str)
Base.codeunit(cns :: Cnstring, i :: Integer) = codeunit(cns.str, i)
