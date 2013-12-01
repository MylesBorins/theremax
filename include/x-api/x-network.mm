/*----------------------------------------------------------------------------
  MCD-X: General API for audio/graphics/interaction programming
         (sibling of MCD-Y API)

  Copyright (c) 2013 Ge Wang
    All rights reserved.
    http://ccrma.stanford.edu/~ge/

  Music, Computing, Design Group @ CCRMA, Stanford University
    http://ccrma.stanford.edu/groups/mcd/

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  U.S.A.
-----------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// name: x-network.mm
// desc: general network and http routines
//
// authors: Ge Wang (ge@ccrma.stanford.edu)
//          Jieun Oh (jieun5@ccrma.stanford.edu)
//    date: Spring 2010
//-----------------------------------------------------------------------------
#include "x-network.h"




// global stuff
static int g_HttpTimeoutMilliSeconds = 2000; // 800;  // jieun: changed from 0
static int g_SimulateNetworkFailure = 0;




//-----------------------------------------------------------------------------
// name: SMHttpPost()
// desc: sends data to the given url
// shouldAbort: the address of a boolean that you can set to true when the post 
//              should be aborted pass null if you don't need this feature
//-----------------------------------------------------------------------------
XHttpPostResult * XHttpPost( CFStringRef url, CFDataRef postData, bool* shouldAbort )
{
	XHttpPostResult *result = new XHttpPostResult;

	CFURLRef myURL = CFURLCreateWithString(kCFAllocatorDefault, url, NULL);
	CFHTTPMessageRef myRequest = CFHTTPMessageCreateRequest(kCFAllocatorDefault, CFSTR("POST"), myURL, kCFHTTPVersion1_1);
	CFHTTPMessageSetBody(myRequest, postData);
	CFStringRef myContentLength = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%ld"), CFDataGetLength(postData));
	CFHTTPMessageSetHeaderFieldValue(myRequest, CFSTR("Content-Length"), myContentLength);
    
	CFReadStreamRef myReadStream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, myRequest);
	CFReadStreamOpen(myReadStream);
    
	BOOL doneReading = NO;
	CFIndex numBytesRead;
	UInt8 buf[4096];
	
	CFMutableDataRef output = CFDataCreateMutable(kCFAllocatorDefault, 0);
	
    long bytesWritten= 0, previousBytesWritten = 0;
    float bytestoWriteInv = 1.0f;
    if (postData)
    {
        long dataLen= [(__bridge NSData*)postData length];
        if (dataLen > 0)
        {
            bytestoWriteInv= 1.0f / dataLen;
        }
    }
    
    int milliSecondsSlept= 0;
    bool aborted= NO;
	while (!doneReading)
    {
        if (shouldAbort != nil)
        {
            // TODO: this is where helper used to be - do we need it?

            if (*shouldAbort)
            {
                NSLog( @"aborting XHttpPost");
                
                aborted= YES;
                break;
            }
        }
        
        if (!CFReadStreamHasBytesAvailable(myReadStream) || g_SimulateNetworkFailure) 
        {
            usleep(36000);
            milliSecondsSlept += 36;
            
            if ((g_HttpTimeoutMilliSeconds > 0) && (milliSecondsSlept > g_HttpTimeoutMilliSeconds))
            {
                // check that we have not uploaded anything for the timeout length (in case there is no abort flag given or the helper doesn't want progress
                CFNumberRef cfSize = (CFNumberRef)CFReadStreamCopyProperty(myReadStream, kCFStreamPropertyHTTPRequestBytesWrittenCount);
                CFNumberGetValue(cfSize, kCFNumberLongType, &bytesWritten);
                CFRelease(cfSize);
                cfSize = NULL;
                
                if (bytesWritten > previousBytesWritten)
                {
                    previousBytesWritten = bytesWritten;
                    milliSecondsSlept= 0; //we're still uploading
                }
                else
                {
                    // we're stuck, abort
                    // [SMLog log: @"XHttpPost timeout"];
                    NSLog( @"http post: timeout..." );
                    aborted= YES;
                    break;
                }
            }
            continue;
        }
        else
        {
            milliSecondsSlept = 0;
        }
        
		numBytesRead = CFReadStreamRead(myReadStream, buf, 4096);
		if (numBytesRead > 0) {
            milliSecondsSlept= 0;
			CFDataAppendBytes(output, buf, numBytesRead);
		} else {
			doneReading = YES;
		}
		
        if (CFReadStreamGetStatus(myReadStream) == kCFStreamStatusAtEnd) doneReading = YES;
	}
	
	CFHTTPMessageRef myResponse = (aborted) ? nil : (CFHTTPMessageRef) CFReadStreamCopyProperty(myReadStream, kCFStreamPropertyHTTPResponseHeader);
	
	if (myResponse!=nil)
	{
        CFHTTPMessageRef httpHeader= (CFHTTPMessageRef)CFReadStreamCopyProperty (myReadStream, kCFStreamPropertyHTTPResponseHeader);
        CFStringRef encodingString = CFHTTPMessageCopyHeaderFieldValue (httpHeader, (CFStringRef)@"Content-Encoding");
        
        
		CFIndex myErrCode       = CFHTTPMessageGetResponseStatusCode(myResponse);
		result->responseCode    = (SInt32)myErrCode;
		result->resultData      = output;
        result->isCompressed    = (encodingString && (CFStringCompare (encodingString, (CFStringRef)@"deflate", 0) == kCFCompareEqualTo));
        CFRelease(myResponse);
        CFRelease(httpHeader);
        if (encodingString)
            CFRelease(encodingString);
	}
    else
    {
        result->responseCode    = -1;
		result->resultData      = NULL;
        result->isCompressed    = false;
    }
    
	CFRelease(myReadStream);
	CFRelease(myContentLength);
	CFRelease(myRequest);
	CFRelease(myURL);
    
	return result;
}




//-----------------------------------------------------------------------------
// name: XHttpPostResultRelease()
// desc: release post result
//-----------------------------------------------------------------------------
void XHttpPostResultRelease(XHttpPostResult * result)
{
	if (result!=NULL)
	{
		if(result->resultData != NULL)
			CFRelease(result->resultData);
		free(result);
	}
}
