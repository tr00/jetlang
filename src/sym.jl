struct SymbolTable
    len :: Ref{Int}
    map :: Dict{String, Int}
end

const jet_sym_map = SymbolTable(0, Dict{String, Int}())

function jet_get_sym(str :: Cstring)
    get!(jet_sym_map.map, unsafe_string(str)) do
        jet_sym_map.len[] += 1
    end
end
