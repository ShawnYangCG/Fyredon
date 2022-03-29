/****************************************************************************************
 
   Copyright (C) 2016 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxsdk.h
#ifndef _FBXSDK_H_
#define _FBXSDK_H_

/**
  * \mainpage FBX SDK Reference
  * <p>
  * \section welcome Welcome to the FBX SDK Reference
  * The FBX SDK Reference contains reference information on every header file, 
  * namespace, class, method, enum, typedef, variable, and other C++ elements 
  * that comprise the FBX software development kit (SDK).
  * <p>
  * The FBX SDK Reference is organized into the following sections:
  * <ul><li>Class List: an alphabetical list of FBX SDK classes
  *     <li>Class Hierarchy: a textual representation of the FBX SDK class structure
  *     <li>Graphical Class Hierarchy: a graphical representation of the FBX SDK class structure
  *     <li>File List: an alphabetical list of all documented header files</ul>
  * <p>
  * \section otherdocumentation Other Documentation
  * Apart from this reference guide, an FBX SDK Programming Guide and many FBX 
  * SDK examples are also provided.
  * <p>
  * \section aboutFBXSDK About the FBX SDK
  * The FBX SDK is a C++ software development kit (SDK) that lets you import 
  * and export 3D scenes using the Autodesk FBX file format. The FBX SDK 
  * reads FBX files created with FiLMBOX version 2.5 and later and writes FBX 
  * files compatible with MotionBuilder version 6.0 and up. 
  */

#include "FBX/fbxsdk/fbxsdk_def.h"

#ifndef FBXSDK_NAMESPACE_USING
	#define FBXSDK_NAMESPACE_USING 1
#endif

//---------------------------------------------------------------------------------------
//Core Base Includes
#include <FBX/fbxsdk/core/base/fbxarray.h>
#include <FBX/fbxsdk/core/base/fbxbitset.h>
#include <FBX/fbxsdk/core/base/fbxcharptrset.h>
#include <FBX/fbxsdk/core/base/fbxcontainerallocators.h>
#include <FBX/fbxsdk/core/base/fbxdynamicarray.h>
#include <FBX/fbxsdk/core/base/fbxstatus.h>
#include <FBX/fbxsdk/core/base/fbxfile.h>
#ifndef FBXSDK_ENV_WINSTORE
	#include <FBX/fbxsdk/core/base/fbxfolder.h>
#endif
#include <FBX/fbxsdk/core/base/fbxhashmap.h>
#include <FBX/fbxsdk/core/base/fbxintrusivelist.h>
#include <FBX/fbxsdk/core/base/fbxmap.h>
#include <FBX/fbxsdk/core/base/fbxmemorypool.h>
#include <FBX/fbxsdk/core/base/fbxpair.h>
#include <FBX/fbxsdk/core/base/fbxset.h>
#include <FBX/fbxsdk/core/base/fbxstring.h>
#include <FBX/fbxsdk/core/base/fbxstringlist.h>
#include <FBX/fbxsdk/core/base/fbxtime.h>
#include <FBX/fbxsdk/core/base/fbxtimecode.h>
#include <FBX/fbxsdk/core/base/fbxutils.h>

//---------------------------------------------------------------------------------------
//Core Math Includes
#include <FBX/fbxsdk/core/math/fbxmath.h>
#include <FBX/fbxsdk/core/math/fbxdualquaternion.h>
#include <FBX/fbxsdk/core/math/fbxmatrix.h>
#include <FBX/fbxsdk/core/math/fbxquaternion.h>
#include <FBX/fbxsdk/core/math/fbxvector2.h>
#include <FBX/fbxsdk/core/math/fbxvector4.h>

//---------------------------------------------------------------------------------------
//Core Sync Includes
#ifndef FBXSDK_ENV_WINSTORE
	#include <FBX/fbxsdk/core/sync/fbxatomic.h>
	#include <FBX/fbxsdk/core/sync/fbxclock.h>
	#include <FBX/fbxsdk/core/sync/fbxsync.h>
	#include <FBX/fbxsdk/core/sync/fbxthread.h>
#endif /* !FBXSDK_ENV_WINSTORE */

//---------------------------------------------------------------------------------------
//Core Includes
#include <FBX/fbxsdk/core/fbxclassid.h>
#include <FBX/fbxsdk/core/fbxconnectionpoint.h>
#include <FBX/fbxsdk/core/fbxdatatypes.h>
#ifndef FBXSDK_ENV_WINSTORE
	#include <FBX/fbxsdk/core/fbxmodule.h>
	#include <FBX/fbxsdk/core/fbxloadingstrategy.h>
#endif /* !FBXSDK_ENV_WINSTORE */
#include <FBX/fbxsdk/core/fbxmanager.h>
#include <FBX/fbxsdk/core/fbxobject.h>
#include <FBX/fbxsdk/core/fbxperipheral.h>
#ifndef FBXSDK_ENV_WINSTORE
	#include <FBX/fbxsdk/core/fbxplugin.h>
	#include <FBX/fbxsdk/core/fbxplugincontainer.h>
#endif /* !FBXSDK_ENV_WINSTORE */
#include <FBX/fbxsdk/core/fbxproperty.h>
#include <FBX/fbxsdk/core/fbxpropertydef.h>
#include <FBX/fbxsdk/core/fbxpropertyhandle.h>
#include <FBX/fbxsdk/core/fbxpropertypage.h>
#include <FBX/fbxsdk/core/fbxpropertytypes.h>
#include <FBX/fbxsdk/core/fbxquery.h>
#include <FBX/fbxsdk/core/fbxqueryevent.h>
#ifndef FBXSDK_ENV_WINSTORE
	#include <FBX/fbxsdk/core/fbxscopedloadingdirectory.h>
	#include <FBX/fbxsdk/core/fbxscopedloadingfilename.h>
#endif /* !FBXSDK_ENV_WINSTORE */
#include <FBX/fbxsdk/core/fbxxref.h>

//---------------------------------------------------------------------------------------
//File I/O Includes
#include <FBX/fbxsdk/fileio/fbxexporter.h>
#include <FBX/fbxsdk/fileio/fbxexternaldocreflistener.h>
#include <FBX/fbxsdk/fileio/fbxfiletokens.h>
#include <FBX/fbxsdk/fileio/fbxglobalcamerasettings.h>
#include <FBX/fbxsdk/fileio/fbxgloballightsettings.h>
#include <FBX/fbxsdk/fileio/fbxgobo.h>
#include <FBX/fbxsdk/fileio/fbximporter.h>
#include <FBX/fbxsdk/fileio/fbxiobase.h>
#include <FBX/fbxsdk/fileio/fbxiopluginregistry.h>
#include <FBX/fbxsdk/fileio/fbxiosettings.h>
#include <FBX/fbxsdk/fileio/fbxstatisticsfbx.h>
#include <FBX/fbxsdk/fileio/fbxstatistics.h>
#include <FBX/fbxsdk/fileio/fbxcallbacks.h>

//---------------------------------------------------------------------------------------
//Scene Includes
#include <FBX/fbxsdk/scene/fbxaudio.h>
#include <FBX/fbxsdk/scene/fbxaudiolayer.h>
#include <FBX/fbxsdk/scene/fbxcollection.h>
#include <FBX/fbxsdk/scene/fbxcollectionexclusive.h>
#include <FBX/fbxsdk/scene/fbxcontainer.h>
#include <FBX/fbxsdk/scene/fbxcontainertemplate.h>
#include <FBX/fbxsdk/scene/fbxdisplaylayer.h>
#include <FBX/fbxsdk/scene/fbxdocument.h>
#include <FBX/fbxsdk/scene/fbxdocumentinfo.h>
#include <FBX/fbxsdk/scene/fbxenvironment.h>
#include <FBX/fbxsdk/scene/fbxgroupname.h>
#include <FBX/fbxsdk/scene/fbxlibrary.h>
#include <FBX/fbxsdk/scene/fbxmediaclip.h>
#include <FBX/fbxsdk/scene/fbxobjectmetadata.h>
#include <FBX/fbxsdk/scene/fbxpose.h>
#include <FBX/fbxsdk/scene/fbxreference.h>
#include <FBX/fbxsdk/scene/fbxscene.h>
#include <FBX/fbxsdk/scene/fbxselectionset.h>
#include <FBX/fbxsdk/scene/fbxselectionnode.h>
#include <FBX/fbxsdk/scene/fbxtakeinfo.h>
#include <FBX/fbxsdk/scene/fbxthumbnail.h>
#include <FBX/fbxsdk/scene/fbxvideo.h>

//---------------------------------------------------------------------------------------
//Scene Animation Includes
#include <FBX/fbxsdk/scene/animation/fbxanimcurve.h>
#include <FBX/fbxsdk/scene/animation/fbxanimcurvebase.h>
#include <FBX/fbxsdk/scene/animation/fbxanimcurvefilters.h>
#include <FBX/fbxsdk/scene/animation/fbxanimcurvenode.h>
#include <FBX/fbxsdk/scene/animation/fbxanimevalclassic.h>
#include <FBX/fbxsdk/scene/animation/fbxanimevalstate.h>
#include <FBX/fbxsdk/scene/animation/fbxanimevaluator.h>
#include <FBX/fbxsdk/scene/animation/fbxanimlayer.h>
#include <FBX/fbxsdk/scene/animation/fbxanimstack.h>
#include <FBX/fbxsdk/scene/animation/fbxanimutilities.h>

//---------------------------------------------------------------------------------------
//Scene Constraint Includes
#include <FBX/fbxsdk/scene/constraint/fbxcharacternodename.h>
#include <FBX/fbxsdk/scene/constraint/fbxcharacter.h>
#include <FBX/fbxsdk/scene/constraint/fbxcharacterpose.h>
#include <FBX/fbxsdk/scene/constraint/fbxconstraint.h>
#include <FBX/fbxsdk/scene/constraint/fbxconstraintaim.h>
#include <FBX/fbxsdk/scene/constraint/fbxconstraintcustom.h>
#include <FBX/fbxsdk/scene/constraint/fbxconstraintparent.h>
#include <FBX/fbxsdk/scene/constraint/fbxconstraintposition.h>
#include <FBX/fbxsdk/scene/constraint/fbxconstraintrotation.h>
#include <FBX/fbxsdk/scene/constraint/fbxconstraintscale.h>
#include <FBX/fbxsdk/scene/constraint/fbxconstraintsinglechainik.h>
#include <FBX/fbxsdk/scene/constraint/fbxconstraintutils.h>
#include <FBX/fbxsdk/scene/constraint/fbxcontrolset.h>
#include <FBX/fbxsdk/scene/constraint/fbxhik2fbxcharacter.h>

//---------------------------------------------------------------------------------------
//Scene Geometry Includes
#include <FBX/fbxsdk/scene/geometry/fbxblendshape.h>
#include <FBX/fbxsdk/scene/geometry/fbxblendshapechannel.h>
#include <FBX/fbxsdk/scene/geometry/fbxcache.h>
#include <FBX/fbxsdk/scene/geometry/fbxcachedeffect.h>
#include <FBX/fbxsdk/scene/geometry/fbxcamera.h>
#include <FBX/fbxsdk/scene/geometry/fbxcamerastereo.h>
#include <FBX/fbxsdk/scene/geometry/fbxcameraswitcher.h>
#include <FBX/fbxsdk/scene/geometry/fbxcluster.h>
#include <FBX/fbxsdk/scene/geometry/fbxdeformer.h>
#include <FBX/fbxsdk/scene/geometry/fbxgenericnode.h>
#include <FBX/fbxsdk/scene/geometry/fbxgeometry.h>
#include <FBX/fbxsdk/scene/geometry/fbxgeometrybase.h>
#include <FBX/fbxsdk/scene/geometry/fbxgeometryweightedmap.h>
#include <FBX/fbxsdk/scene/geometry/fbxlight.h>
#include <FBX/fbxsdk/scene/geometry/fbxlimitsutilities.h>
#include <FBX/fbxsdk/scene/geometry/fbxline.h>
#include <FBX/fbxsdk/scene/geometry/fbxlodgroup.h>
#include <FBX/fbxsdk/scene/geometry/fbxmarker.h>
#include <FBX/fbxsdk/scene/geometry/fbxmesh.h>
#include <FBX/fbxsdk/scene/geometry/fbxnode.h>
#include <FBX/fbxsdk/scene/geometry/fbxnodeattribute.h>
#include <FBX/fbxsdk/scene/geometry/fbxnull.h>
#include <FBX/fbxsdk/scene/geometry/fbxnurbs.h>
#include <FBX/fbxsdk/scene/geometry/fbxnurbscurve.h>
#include <FBX/fbxsdk/scene/geometry/fbxnurbssurface.h>
#include <FBX/fbxsdk/scene/geometry/fbxopticalreference.h>
#include <FBX/fbxsdk/scene/geometry/fbxpatch.h>
#include <FBX/fbxsdk/scene/geometry/fbxproceduralgeometry.h>
#include <FBX/fbxsdk/scene/geometry/fbxshape.h>
#include <FBX/fbxsdk/scene/geometry/fbxskeleton.h>
#include <FBX/fbxsdk/scene/geometry/fbxskin.h>
#include <FBX/fbxsdk/scene/geometry/fbxsubdeformer.h>
#include <FBX/fbxsdk/scene/geometry/fbxsubdiv.h>
#include <FBX/fbxsdk/scene/geometry/fbxtrimnurbssurface.h>
#include <FBX/fbxsdk/scene/geometry/fbxvertexcachedeformer.h>
#include <FBX/fbxsdk/scene/geometry/fbxweightedmapping.h>

//---------------------------------------------------------------------------------------
//Scene Shading Includes
#include <FBX/fbxsdk/scene/shading/fbxshadingconventions.h>
#include <FBX/fbxsdk/scene/shading/fbxbindingsentryview.h>
#include <FBX/fbxsdk/scene/shading/fbxbindingtable.h>
#include <FBX/fbxsdk/scene/shading/fbxbindingtableentry.h>
#include <FBX/fbxsdk/scene/shading/fbxbindingoperator.h>
#include <FBX/fbxsdk/scene/shading/fbxconstantentryview.h>
#include <FBX/fbxsdk/scene/shading/fbxentryview.h>
#include <FBX/fbxsdk/scene/shading/fbxfiletexture.h>
#include <FBX/fbxsdk/scene/shading/fbximplementation.h>
#include <FBX/fbxsdk/scene/shading/fbximplementationfilter.h>
#include <FBX/fbxsdk/scene/shading/fbximplementationutils.h>
#include <FBX/fbxsdk/scene/shading/fbxlayeredtexture.h>
#include <FBX/fbxsdk/scene/shading/fbxoperatorentryview.h>
#include <FBX/fbxsdk/scene/shading/fbxproceduraltexture.h>
#include <FBX/fbxsdk/scene/shading/fbxpropertyentryview.h>
#include <FBX/fbxsdk/scene/shading/fbxsemanticentryview.h>
#include <FBX/fbxsdk/scene/shading/fbxsurfacelambert.h>
#include <FBX/fbxsdk/scene/shading/fbxsurfacematerial.h>
#include <FBX/fbxsdk/scene/shading/fbxsurfacematerialutils.h>
#include <FBX/fbxsdk/scene/shading/fbxsurfacephong.h>
#include <FBX/fbxsdk/scene/shading/fbxtexture.h>

//---------------------------------------------------------------------------------------
//Utilities Includes
#include <FBX/fbxsdk/utils/fbxdeformationsevaluator.h>
#include <FBX/fbxsdk/utils/fbxprocessor.h>
#include <FBX/fbxsdk/utils/fbxprocessorxref.h>
#include <FBX/fbxsdk/utils/fbxprocessorxrefuserlib.h>
#include <FBX/fbxsdk/utils/fbxprocessorshaderdependency.h>
#include <FBX/fbxsdk/utils/fbxclonemanager.h>
#include <FBX/fbxsdk/utils/fbxgeometryconverter.h>
#include <FBX/fbxsdk/utils/fbxmanipulators.h>
#include <FBX/fbxsdk/utils/fbxmaterialconverter.h>
#include <FBX/fbxsdk/utils/fbxrenamingstrategyfbx5.h>
#include <FBX/fbxsdk/utils/fbxrenamingstrategyfbx6.h>
#include <FBX/fbxsdk/utils/fbxrenamingstrategyutilities.h>
#include <FBX/fbxsdk/utils/fbxrootnodeutility.h>
#include <FBX/fbxsdk/utils/fbxusernotification.h>
#include <FBX/fbxsdk/utils/fbxscenecheckutility.h>

//---------------------------------------------------------------------------------------
#if defined(FBXSDK_NAMESPACE) && (FBXSDK_NAMESPACE_USING == 1)
	using namespace FBXSDK_NAMESPACE;
#endif

#endif /* _FBXSDK_H_ */
