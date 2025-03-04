/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 *
 * HDF is dual licensed: you can use it either under the terms of
 * the GPL, or the BSD license, at your option.
 * See the LICENSE file in the root of this repository for complete details.
 */

#include "ast/ast_integer_type.h"

namespace OHOS {
namespace HDI {
bool ASTIntegerType::IsIntegerType()
{
    return true;
}

String ASTIntegerType::ToString()
{
    return "int";
}

TypeKind ASTIntegerType::GetTypeKind()
{
    return TypeKind::TYPE_INT;
}

String ASTIntegerType::EmitCType(TypeMode mode) const
{
    switch (mode) {
        case TypeMode::NO_MODE:
            return "int32_t";
        case TypeMode::PARAM_IN:
            return "int32_t";
        case TypeMode::PARAM_OUT:
            return "int32_t*";
        case TypeMode::LOCAL_VAR:
            return "int32_t";
        default:
            return "unknow type";
    }
}

String ASTIntegerType::EmitCppType(TypeMode mode) const
{
    switch (mode) {
        case TypeMode::NO_MODE:
            return "int32_t";
        case TypeMode::PARAM_IN:
            return "int32_t";
        case TypeMode::PARAM_OUT:
            return "int32_t&";
        case TypeMode::LOCAL_VAR:
            return "int32_t";
        default:
            return "unknow type";
    }
}

String ASTIntegerType::EmitJavaType(TypeMode mode, bool isInnerType) const
{
    return isInnerType ? "Integer" : "int";
}

void ASTIntegerType::EmitCWriteVar(const String& parcelName, const String& name, const String& gotoLabel,
    StringBuilder& sb, const String& prefix) const
{
    sb.Append(prefix).AppendFormat("if (!HdfSbufWriteInt32(%s, %s)) {\n",
        parcelName.string(), name.string());
    sb.Append(prefix + g_tab).AppendFormat(
        "HDF_LOGE(\"%%{public}s: write %s failed!\", __func__);\n", name.string());
    sb.Append(prefix + g_tab).Append("ec = HDF_ERR_INVALID_PARAM;\n");
    sb.Append(prefix + g_tab).AppendFormat("goto %s;\n", gotoLabel.string());
    sb.Append(prefix).Append("}\n");
}

void ASTIntegerType::EmitCProxyReadVar(const String& parcelName, const String& name, bool isInnerType,
    const String& gotoLabel, StringBuilder& sb, const String& prefix) const
{
    sb.Append(prefix).AppendFormat("if (!HdfSbufReadInt32(%s, %s)) {\n", parcelName.string(), name.string());
    sb.Append(prefix + g_tab).AppendFormat(
        "HDF_LOGE(\"%%{public}s: read %s failed!\", __func__);\n", name.string());
    sb.Append(prefix + g_tab).Append("ec = HDF_ERR_INVALID_PARAM;\n");
    sb.Append(prefix + g_tab).AppendFormat("goto %s;\n", gotoLabel.string());
    sb.Append(prefix).Append("}\n");
}

void ASTIntegerType::EmitCStubReadVar(const String& parcelName, const String& name, StringBuilder& sb,
    const String& prefix) const
{
    sb.Append(prefix).AppendFormat("if (!HdfSbufReadInt32(%s, %s)) {\n", parcelName.string(), name.string());
    sb.Append(prefix + g_tab).AppendFormat(
        "HDF_LOGE(\"%%{public}s: read %s failed!\", __func__);\n", name.string());
    sb.Append(prefix + g_tab).Append("ec = HDF_ERR_INVALID_PARAM;\n");
    sb.Append(prefix + g_tab).Append("goto errors;\n");
    sb.Append(prefix).Append("}\n");
}

void ASTIntegerType::EmitCppWriteVar(const String& parcelName, const String& name, StringBuilder& sb,
    const String& prefix, unsigned int innerLevel) const
{
    sb.Append(prefix).AppendFormat("if (!%s.WriteInt32(%s)) {\n", parcelName.string(), name.string());
    sb.Append(prefix + g_tab).AppendFormat(
        "HDF_LOGE(\"%%{public}s: write %s failed!\", __func__);\n", name.string());
    sb.Append(prefix + g_tab).Append("return HDF_ERR_INVALID_PARAM;\n");
    sb.Append(prefix).Append("}\n");
}

void ASTIntegerType::EmitCppReadVar(const String& parcelName, const String& name, StringBuilder& sb,
    const String& prefix, bool initVariable, unsigned int innerLevel) const
{
    if (initVariable) {
        sb.Append(prefix).AppendFormat("%s %s = %s.ReadInt32();\n", EmitCppType().string(),
            name.string(), parcelName.string());
    } else {
        sb.Append(prefix).AppendFormat("%s = %s.ReadInt32();\n", name.string(), parcelName.string());
    }
}

void ASTIntegerType::EmitCMarshalling(const String& name, StringBuilder& sb, const String& prefix) const
{
    sb.Append(prefix).AppendFormat("if (!HdfSbufWriteInt32(data, %s)) {\n", name.string());
    sb.Append(prefix + g_tab).AppendFormat(
        "HDF_LOGE(\"%%{public}s: write %s failed!\", __func__);\n", name.string());
    sb.Append(prefix + g_tab).Append("return false;\n");
    sb.Append(prefix).Append("}\n");
}

void ASTIntegerType::EmitCUnMarshalling(const String& name, StringBuilder& sb, const String& prefix,
    std::vector<String>& freeObjStatements) const
{
    sb.Append(prefix).AppendFormat("if (!HdfSbufReadInt32(data, &%s)) {\n", name.string());
    sb.Append(prefix + g_tab).AppendFormat(
        "HDF_LOGE(\"%%{public}s: read %s failed!\", __func__);\n", name.string());
    EmitFreeStatements(freeObjStatements, sb, prefix + g_tab);
    sb.Append(prefix + g_tab).Append("goto errors;\n");
    sb.Append(prefix).Append("}\n");
}

void ASTIntegerType::EmitCppMarshalling(const String& parcelName, const String& name, StringBuilder& sb,
    const String& prefix, unsigned int innerLevel) const
{
    sb.Append(prefix).AppendFormat("if (!%s.WriteInt32(%s)) {\n", parcelName.string(), name.string());
    sb.Append(prefix + g_tab).AppendFormat(
        "HDF_LOGE(\"%%{public}s: write %s failed!\", __func__);\n", name.string());
    sb.Append(prefix + g_tab).Append("return false;\n");
    sb.Append(prefix).Append("}\n");
}

void ASTIntegerType::EmitCppUnMarshalling(const String& parcelName, const String& name, StringBuilder& sb,
    const String& prefix, bool emitType, unsigned int innerLevel) const
{
    if (emitType) {
        sb.Append(prefix).AppendFormat("%s %s = %s.ReadInt32();\n",
            EmitCppType().string(), name.string(), parcelName.string());
    } else {
        sb.Append(prefix).AppendFormat("%s = %s.ReadInt32();\n", name.string(), parcelName.string());
    }
}
} // namespace HDI
} // namespace OHOS