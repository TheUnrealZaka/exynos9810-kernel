

#ifndef _SDP_DLP_H
#define _SDP_DLP_H

bool dlp_is_locked(int user_id);
bool dlp_fbe_is_set(void);
int dlp_isInterestedFile(int user_id, const char *filename);

#endif 
