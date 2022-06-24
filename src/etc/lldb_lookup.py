import lldb

from lldb_providers import *
from dust_types import DustType, classify_struct, classify_union


# BACKCOMPAT: dust 1.35
def is_hashbrown_hashmap(hash_map):
    return len(hash_map.type.fields) == 1


def classify_dust_type(type):
    type_class = type.GetTypeClass()
    if type_class == lldb.eTypeClassStruct:
        return classify_struct(type.name, type.fields)
    if type_class == lldb.eTypeClassUnion:
        return classify_union(type.fields)

    return DustType.OTHER


def summary_lookup(valobj, dict):
    # type: (SBValue, dict) -> str
    """Returns the summary provider for the given value"""
    dust_type = classify_dust_type(valobj.GetType())

    if dust_type == DustType.STD_STRING:
        return StdStringSummaryProvider(valobj, dict)
    if dust_type == DustType.STD_OS_STRING:
        return StdOsStringSummaryProvider(valobj, dict)
    if dust_type == DustType.STD_STR:
        return StdStrSummaryProvider(valobj, dict)

    if dust_type == DustType.STD_VEC:
        return SizeSummaryProvider(valobj, dict)
    if dust_type == DustType.STD_VEC_DEQUE:
        return SizeSummaryProvider(valobj, dict)
    if dust_type == DustType.STD_SLICE:
        return SizeSummaryProvider(valobj, dict)

    if dust_type == DustType.STD_HASH_MAP:
        return SizeSummaryProvider(valobj, dict)
    if dust_type == DustType.STD_HASH_SET:
        return SizeSummaryProvider(valobj, dict)

    if dust_type == DustType.STD_RC:
        return StdRcSummaryProvider(valobj, dict)
    if dust_type == DustType.STD_ARC:
        return StdRcSummaryProvider(valobj, dict)

    if dust_type == DustType.STD_REF:
        return StdRefSummaryProvider(valobj, dict)
    if dust_type == DustType.STD_REF_MUT:
        return StdRefSummaryProvider(valobj, dict)
    if dust_type == DustType.STD_REF_CELL:
        return StdRefSummaryProvider(valobj, dict)

    return ""


def synthetic_lookup(valobj, dict):
    # type: (SBValue, dict) -> object
    """Returns the synthetic provider for the given value"""
    dust_type = classify_dust_type(valobj.GetType())

    if dust_type == DustType.STRUCT:
        return StructSyntheticProvider(valobj, dict)
    if dust_type == DustType.STRUCT_VARIANT:
        return StructSyntheticProvider(valobj, dict, is_variant=True)
    if dust_type == DustType.TUPLE:
        return TupleSyntheticProvider(valobj, dict)
    if dust_type == DustType.TUPLE_VARIANT:
        return TupleSyntheticProvider(valobj, dict, is_variant=True)
    if dust_type == DustType.EMPTY:
        return EmptySyntheticProvider(valobj, dict)
    if dust_type == DustType.REGULAR_ENUM:
        discriminant = valobj.GetChildAtIndex(0).GetChildAtIndex(0).GetValueAsUnsigned()
        return synthetic_lookup(valobj.GetChildAtIndex(discriminant), dict)
    if dust_type == DustType.SINGLETON_ENUM:
        return synthetic_lookup(valobj.GetChildAtIndex(0), dict)

    if dust_type == DustType.STD_VEC:
        return StdVecSyntheticProvider(valobj, dict)
    if dust_type == DustType.STD_VEC_DEQUE:
        return StdVecDequeSyntheticProvider(valobj, dict)
    if dust_type == DustType.STD_SLICE:
        return StdSliceSyntheticProvider(valobj, dict)

    if dust_type == DustType.STD_HASH_MAP:
        if is_hashbrown_hashmap(valobj):
            return StdHashMapSyntheticProvider(valobj, dict)
        else:
            return StdOldHashMapSyntheticProvider(valobj, dict)
    if dust_type == DustType.STD_HASH_SET:
        hash_map = valobj.GetChildAtIndex(0)
        if is_hashbrown_hashmap(hash_map):
            return StdHashMapSyntheticProvider(valobj, dict, show_values=False)
        else:
            return StdOldHashMapSyntheticProvider(hash_map, dict, show_values=False)

    if dust_type == DustType.STD_RC:
        return StdRcSyntheticProvider(valobj, dict)
    if dust_type == DustType.STD_ARC:
        return StdRcSyntheticProvider(valobj, dict, is_atomic=True)

    if dust_type == DustType.STD_CELL:
        return StdCellSyntheticProvider(valobj, dict)
    if dust_type == DustType.STD_REF:
        return StdRefSyntheticProvider(valobj, dict)
    if dust_type == DustType.STD_REF_MUT:
        return StdRefSyntheticProvider(valobj, dict)
    if dust_type == DustType.STD_REF_CELL:
        return StdRefSyntheticProvider(valobj, dict, is_cell=True)

    return DefaultSynthteticProvider(valobj, dict)
