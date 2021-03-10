//
//  SHPathUtils.h
//  Sourcehold
//
//  Created by apodrugin on 08.02.2021.
//

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

bool SHQueryUserDocumentsDirectoryPath(char *outBuffer, unsigned long outBufferSize);
bool SHQueryMainBundleResourcesDirectoryPath(char *outBuffer, unsigned long outBufferSize);

#ifdef __cplusplus
}
#endif // __cplusplus
