//
//  SHPathUtils.h
//  Sourcehold
//
//  Created by apodrugin on 08.02.2021.
//

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

bool SHGetUserDocumentsDirectoryPath(char *buffer, unsigned long bufferSize);
bool SHGetMainBundleResourcesDirectoryPath(char *buffer, unsigned long bufferSize);

#ifdef __cplusplus
}
#endif // __cplusplus
