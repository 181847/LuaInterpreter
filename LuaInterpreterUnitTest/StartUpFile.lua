-- this file is used to test the luaInterpreter

test_table = {a = 1, b = 2, c = 3, 4, 5, 6}

test_ToNumberAndPop_1 = 1
test_ToNumberAndPop_2 = 2

test_ToIntegerAndPop_1 = 3
test_ToIntegerAndPop_2 = 4

test_ToStringandClear = 'test_string_5'

test_GetGlobal = 6

-- use a meshdata to test userdata extract
md = require("MeshData")
test_userdata_meshdata = md.new()
-- 1 position
test_userdata_meshdata:addPosition(1, 2, 3)
-- 2 normal
test_userdata_meshdata:addTextureCoord(1, 2)
test_userdata_meshdata:addTextureCoord(1, 2)
-- 3 tangentU
test_userdata_meshdata:addNormal(1, 2, 3)
test_userdata_meshdata:addNormal(1, 2, 3)
test_userdata_meshdata:addNormal(1, 2, 3)
-- 4 texturecooor
test_userdata_meshdata:addTangentU(1, 2, 3)
test_userdata_meshdata:addTangentU(1, 2, 3)
test_userdata_meshdata:addTangentU(1, 2, 3)
test_userdata_meshdata:addTangentU(1, 2, 3)
-- 5 vertices
test_userdata_meshdata:addVertex(1, 2, 3, 4)
test_userdata_meshdata:addVertex(1, 2, 3, 4)
test_userdata_meshdata:addVertex(1, 2, 3, 4)
test_userdata_meshdata:addVertex(1, 2, 3, 4)
test_userdata_meshdata:addVertex(1, 2, 3, 4)
-- 6 indices
test_userdata_meshdata:addIndex(1)
test_userdata_meshdata:addIndex(1)
test_userdata_meshdata:addIndex(1)
test_userdata_meshdata:addIndex(1)
test_userdata_meshdata:addIndex(1)
test_userdata_meshdata:addIndex(1)
