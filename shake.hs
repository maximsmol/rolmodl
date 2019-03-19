{-# LANGUAGE TypeApplications #-} -- generic-lens
{-# LANGUAGE DataKinds #-} -- type-level strings
{-# LANGUAGE DeriveGeneric #-} -- generics
{-# LANGUAGE DeriveAnyClass #-} -- data-default
{-# LANGUAGE DuplicateRecordFields #-} -- generic-lens only handles the use site

import Control.Monad(forM_, join)
import Data.List(group, sort, (\\), stripPrefix)
import Data.Maybe(fromMaybe)

import System.FilePath.Glob
import GHC.Generics
import Data.Default
import Control.Lens hiding ((<.>))
import Data.Generics.Product
import System.Directory(createDirectoryIfMissing, getCurrentDirectory)
import Data.List.Extra(stripSuffix)

import Development.Shake
import Development.Shake.FilePath
import Development.Shake.Util


projectCfg :: ProjectCfg
projectCfg = def

main :: IO ()
main = shakeArgs (shakeOptions' (def :: DirCfg)) $ do
  cwd <- liftIO $ getCurrentDirectory
  let dircfg = projectCfg^.field @"dircfg"

  action $ do
    liftIO $ createDirectoryIfMissing True (dircfg^.field @"bld"</>"report")

  -- base
  let rolmodlDistPath = dircfg^.field @"dist"
  let rolmodlLibPath = rolmodlDistPath</>"librolmodl.dylib"
  let rolmodlTestPath = rolmodlDistPath</>"test"
  let rolmodlTestSWTexPath = rolmodlDistPath</>"test_swtex"

  -- rolmodl
  let rolmodlLibCPPTargetCfg =
        (def :: CPPTargetCfg)
          & field @"name" .~ "rolmodl"
          & field @"publicCPPObjs" .~ ["librolmodl.dylib"]
          & field @"publicIncludeDirs" .~ ["hpp"]
  let rolmodlLibCPPSourceCfg =
        (def :: CPPSourceCfg)
          & field @"fromInTarget" .~ ((<.> "cpp") <$> ["Base", "Event", "Kb", "Mouse", "PixelFmt", "Ren", "Tex", "Win", "SWTex"])
  let rolmodlLibCPPIncludeListing =
        (def :: CPPIncludeListing)
          & field @"fromInTarget" .~ ["."]
  let rolmodlLibSysCPPIncludeListing =
        (def :: CPPIncludeListing)
          & field @"fromWorld" .~ ["SDL2"]
  let rolmodlLibCPPIncludeCfg =
        (def :: CPPIncludeCfg)
          & field @"warn" .~ rolmodlLibCPPIncludeListing
          & field @"noWarn" .~ rolmodlLibSysCPPIncludeListing
  let rolmodlLibCPPObjBuildCfg =
        (def :: CPPObjBuildCfg)
          & field @"target" .~ rolmodlLibCPPTargetCfg
          & field @"srcs" .~ rolmodlLibCPPSourceCfg
          & field @"includes" .~ rolmodlLibCPPIncludeCfg
  genCPPObjBuildRules rolmodlLibCPPObjBuildCfg

  let rolmodlLibCPPObjSourceCfg =
        (def :: CPPObjSourceCfg)
          & field @"fromCPPObjBuild" .~ [rolmodlLibCPPObjBuildCfg]
          & field @"fromWorld" .~ ["libSDL2.dylib"]
  let rolmodlLibCPPLinkBuildFlagCfg =
        defaultCPPLinkBuildFlagCfg
          & field @"common" <>~ ["-shared", "-install_name", "@rpath/librolmodl.dylib"]
  let rolmodlLibCPPLinkBuildCfg =
        (def :: CPPLinkBuildCfg)
          & field @"name" .~ "librolmodl.dylib"
          & field @"target" .~ rolmodlLibCPPTargetCfg
          & field @"objs" .~ rolmodlLibCPPObjSourceCfg
          & field @"flags" .~ rolmodlLibCPPLinkBuildFlagCfg
  genCPPLinkBuildRules rolmodlLibCPPLinkBuildCfg

  linkToDistRules (getCPPLinkPrimaryBuildOut rolmodlLibCPPLinkBuildCfg) rolmodlLibPath

  -- test
  let rolmodlTestCPPTargetCfg =
        (def :: CPPTargetCfg)
          & field @"name" .~ "test"
  let rolmodlTestCPPSourceCfg =
        (def :: CPPSourceCfg)
          & field @"fromInTarget" .~ ((<.> "cpp") <$> ["main"])
  let rolmodlTestCPPIncludeListing =
        (def :: CPPIncludeListing)
          & field @"fromInTarget" .~ ["."]
          & field @"fromOtherTarget" .~ [rolmodlLibCPPTargetCfg]
  let rolmodlTestSysCPPIncludeListing =
        (def :: CPPIncludeListing)
          & field @"fromWorld" .~ ["SDL2"]
  let rolmodlTestCPPIncludeCfg =
        (def :: CPPIncludeCfg)
          & field @"warn" .~ rolmodlTestCPPIncludeListing
          & field @"noWarn" .~ rolmodlTestSysCPPIncludeListing
  let rolmodlTestCPPObjBuildCfg =
        (def :: CPPObjBuildCfg)
          & field @"target" .~ rolmodlTestCPPTargetCfg
          & field @"srcs" .~ rolmodlTestCPPSourceCfg
          & field @"includes" .~ rolmodlTestCPPIncludeCfg
  genCPPObjBuildRules rolmodlTestCPPObjBuildCfg

  let rolmodlTestCPPObjSourceCfg =
        (def :: CPPObjSourceCfg)
          & field @"fromCPPObjBuild" .~ [rolmodlTestCPPObjBuildCfg]
          & field @"fromCPPLinkBuild" .~ [rolmodlLibCPPLinkBuildCfg]
          & field @"fromWorld" .~ ["libSDL2.dylib"]
  let rolmodlTestCPPLinkBuildFlagCfg =
        defaultCPPLinkBuildFlagCfg
          & field @"common" <>~ ["-rpath", "@executable_path"]
  let rolmodlTestCPPLinkBuildCfg =
        (def :: CPPLinkBuildCfg)
          & field @"name" .~ "test"
          & field @"target" .~ rolmodlTestCPPTargetCfg
          & field @"objs" .~ rolmodlTestCPPObjSourceCfg
          & field @"flags" .~ rolmodlTestCPPLinkBuildFlagCfg
  genCPPLinkBuildRules rolmodlTestCPPLinkBuildCfg


  let rolmodlTestSWTexCPPTargetCfg =
        (def :: CPPTargetCfg)
          & field @"name" .~ "test"
  let rolmodlTestSWTexCPPSourceCfg =
        rolmodlTestCPPSourceCfg
          & field @"fromInTarget" .~ ((<.> "cpp") <$> ["main_swtex"])
  let rolmodlTestSWTexCPPObjBuildCfg =
        (def :: CPPObjBuildCfg)
          & field @"target" .~ rolmodlTestSWTexCPPTargetCfg
          & field @"srcs" .~ rolmodlTestSWTexCPPSourceCfg
          & field @"includes" .~ rolmodlTestCPPIncludeCfg
  genCPPObjBuildRules rolmodlTestSWTexCPPObjBuildCfg

  let rolmodlTestSWTexCPPObjSourceCfg =
        (def :: CPPObjSourceCfg)
          & field @"fromCPPObjBuild" .~ [rolmodlTestSWTexCPPObjBuildCfg]
          & field @"fromCPPLinkBuild" .~ [rolmodlLibCPPLinkBuildCfg]
          & field @"fromWorld" .~ ["libSDL2.dylib"]
  let rolmodlTestSWTexCPPLinkBuildCfg =
        (def :: CPPLinkBuildCfg)
          & field @"name" .~ "test_swtex"
          & field @"target" .~ rolmodlTestSWTexCPPTargetCfg
          & field @"objs" .~ rolmodlTestSWTexCPPObjSourceCfg
          & field @"flags" .~ rolmodlTestCPPLinkBuildFlagCfg
  genCPPLinkBuildRules rolmodlTestSWTexCPPLinkBuildCfg

  linkToDistRules' (getCPPLinkPrimaryBuildOut rolmodlTestCPPLinkBuildCfg) rolmodlTestPath $ \out -> do
    cmd_ "install_name_tool" "-add_rpath" [rolmodlDistPath] [out] -- changes the value of the dependency here because the changed file is actually just a link to the dep
    return ()

  linkToDistRules' (getCPPLinkPrimaryBuildOut rolmodlTestSWTexCPPLinkBuildCfg) rolmodlTestSWTexPath $ \out -> do
    cmd_ "install_name_tool" "-add_rpath" [rolmodlDistPath] [out] -- changes the value of the dependency here because the changed file is actually just a link to the dep
    return ()

  want [rolmodlLibPath, rolmodlTestPath, rolmodlTestSWTexPath]

  return ()

{-

ToDo:
- CPP: Check that CPPTargets' public outputs are actually produced
- CPP: Depend on separate CPP headers
- CPP: Depend on system libraries
- CPP: Add precompiled headers
- CPP: Add compile_commands generation
- Take clangCompDB out of ProjectCfg
- Look into record inheritance
- Allow querying actual outputs of targets for dependencies
- Allow for platform-specific compilation
- Look into proper named parameters
- Look into namespacing
- Handle dist outputs better
- Make a proper library out of this file

-}

shakeOptions' :: DirCfg -> ShakeOptions
shakeOptions' dc = shakeOptions{
    shakeFiles = bldPath</>"builddb",
    shakeReport = ((bldPath </> "report") </>) <$> [
      "t"<.>"trace", "h"<.>"html"
    ],
    shakeLint = Just LintBasic,
    shakeTimings = True
  }
  where
    bldPath :: FilePath
    bldPath = projectCfg^.field @"dircfg".field @"bld"


data ProjectCfg = ProjectCfg {
  dircfg :: DirCfg,
  clangCompDB :: String
} deriving (Generic, Show)
instance Default ProjectCfg where
  def = ProjectCfg {dircfg = def, clangCompDB = (def :: DirCfg)^.field @"bld" </> "compile_commands.json"}

data DirCfg = DirCfg {
  src :: FilePath,
  dist :: FilePath,
  bld :: FilePath,
  res :: FilePath,
  world:: FilePath
} deriving (Generic, Show)
instance Default DirCfg where
  def = DirCfg {
    src = "src",
    dist = "dist",
    bld = "bld",
    res = "res",
    world = "world"
  }


data CPPTargetCfg = CPPTargetCfg {
  name :: String,
  dircfgOverride :: Maybe DirCfg,

  publicIncludeDirs :: [FilePath],
  publicCPPObjs :: [FilePath],
  publicCPPSources :: [FilePath]
} deriving (Generic, Default, Show)
defaultDircfgForCPPTargetNamed :: String -> DirCfg
defaultDircfgForCPPTargetNamed name =
  foldr (\l cfg-> over l (</> name) cfg) (projectCfg^.field @"dircfg") [field @"src", field @"bld", field @"dist"]

dircfgForCPPTarget :: CPPTargetCfg -> DirCfg
dircfgForCPPTarget cfg =
  case cfg^.field @"dircfgOverride" of
    Just x -> x
    Nothing -> defaultDircfgForCPPTargetNamed $ cfg^.field @"name"

data CPPIncludeListing = CPPIncludeListing {
  fromInTarget :: [FilePath],
  fromOtherTarget :: [CPPTargetCfg],
  fromWorld :: [FilePath],
  fromUnportablePath :: [FilePath]
} deriving (Generic, Default, Show)
cppIncludeListingToPaths :: CPPTargetCfg -> CPPIncludeListing -> [FilePath]
cppIncludeListingToPaths tcfg ilist =
  join $
    [(ilist^.field @"fromUnportablePath")] ++
    [(tdircfg^.field @"src" </>) <$> (ilist^.field @"fromInTarget")] ++
    [(tdircfg^.field @"world" </>) <$> (ilist^.field @"fromWorld")] ++
    (includePathsForTarget <$> (ilist^.field @"fromOtherTarget"))
  where
    tdircfg :: DirCfg
    tdircfg = dircfgForCPPTarget tcfg

    includePathsForTarget :: CPPTargetCfg -> [FilePath]
    includePathsForTarget tcfg =
      let dircfg = dircfgForCPPTarget tcfg
      in (dircfg^.field @"src" </>) <$> tcfg^.field @"publicIncludeDirs"

data CPPIncludeCfg = CPPIncludeCfg {
  warn :: CPPIncludeListing,
  noWarn :: CPPIncludeListing
} deriving (Generic, Default, Show)


data CPPFlagCfg = CPPFlagCfg {
  common :: [String],
  debug :: [String],
  release :: [String]
} deriving (Generic, Default, Show)

defaultCPPObjectBuildFlagCfg :: CPPFlagCfg
defaultCPPObjectBuildFlagCfg =
  CPPFlagCfg {
    common = defaultCommon,
    debug = ["-O0", "-g", "-glldb"],
    release = ["-Ofast"]
  }
  where
    defaultCommon :: [String]
    defaultCommon =
      let
        warnFlags = [
            "-Weverything", "-Wno-padded", -- "-Wno-float-equal",
            "-Wno-c++98-compat", "-Wno-c++98-c++11-compat", "-Wno-c++98-c++11-compat-pedantic",
            "-Wno-c++98-compat-pedantic", "-Wno-c99-extensions", "-Wno-c++98-c++11-c++14-compat"
          ]
        diagFlags = ["-fcolor-diagnostics"]
      in ["-std=c++1z"] ++ warnFlags ++ diagFlags


data CPPSourceCfg = CPPSourceCfg {
  fromInTarget :: [FilePath],
  fromOtherTarget :: [CPPTargetCfg],
  fromWorld :: [FilePath],
  fromUnportablePath :: [FilePath]
} deriving (Generic, Default, Show)
cppSourceCfgToPatterns :: CPPTargetCfg -> CPPSourceCfg -> [Pattern]
cppSourceCfgToPatterns tcfg cfg =
  let strs = join $
        [(tdircfg^.field @"src"</>) <$> (cfg^.field @"fromInTarget")] ++
        (srcPatternsForTarget <$> (cfg^.field @"fromOtherTarget")) ++
        [(tdircfg^.field @"world"</>) <$> (cfg^.field @"fromWorld")] ++
        [cfg^.field @"fromUnportablePath"]
  in compile <$> strs
  where
    tdircfg :: DirCfg
    tdircfg = dircfgForCPPTarget tcfg

    srcPatternsForTarget :: CPPTargetCfg -> [String]
    srcPatternsForTarget tcfg =
      let dircfg = dircfgForCPPTarget tcfg
      in (dircfg^.field @"bld" </>) <$> tcfg^.field @"publicCPPSources"
cppSourceCfgToPaths :: CPPTargetCfg -> CPPSourceCfg -> IO [FilePath]
cppSourceCfgToPaths tcfg cfg =
  dedup <$> join <$> globDir (cppSourceCfgToPatterns tcfg cfg) "."

data CPPObjBuildCfg = CPPObjBuildCfg {
  target :: CPPTargetCfg,
  dircfgOverride :: Maybe DirCfg,
  srcs :: CPPSourceCfg,

  includes :: CPPIncludeCfg,
  compiler :: String,
  debug :: Bool,
  flags :: CPPFlagCfg
} deriving (Generic, Show)
instance Default CPPObjBuildCfg where
  def = CPPObjBuildCfg {
      target = def,
      dircfgOverride = def,
      srcs = def,

      includes = def,
      compiler = "clang++",
      debug = True,
      flags = defaultCPPObjectBuildFlagCfg
    }

findCPPObjectBuildOuts :: CPPObjBuildCfg -> IO [FilePath]
findCPPObjectBuildOuts cfg = do
  let outFromSrc = \x -> dircfg^.field @"bld"</>dropKnownDirname (dircfg^.field @"src") x-<.>"o"
  (fmap outFromSrc) <$> cppSourceCfgToPaths (cfg^.field @"target") (cfg^.field @"srcs")
  where
    dircfg :: DirCfg
    dircfg = dircfgForCPPObjBuild cfg

genCPPObjBuildRules :: CPPObjBuildCfg -> Rules ()
genCPPObjBuildRules cfg = do
  files <- liftIO $ findCPPObjectBuildOuts cfg

  let depFromOut = \x -> gendDepDir</>dropKnownDirname bldDir x-<.>"dep"
  let outFromDep = \x -> bldDir</>dropKnownDirname gendDepDir x-<.>"o"
  let clangCDBFromOut = \x -> clangCDBDir</>dropKnownDirname bldDir x-<.>"json"
  let outFromClangCDB = \x -> bldDir</>dropKnownDirname clangCDBDir x-<.>"o"

  files |%> \f -> buildFn f (depFromOut f) (clangCDBFromOut f)
  fmap depFromOut files |%> \x -> need [outFromDep x]
  fmap clangCDBFromOut files |%> \x -> need [outFromClangCDB x]

  where
    tcfg :: CPPTargetCfg
    tcfg = cfg^.field @"target"


    dircfg :: DirCfg
    dircfg = dircfgForCPPObjBuild cfg

    bldDir :: FilePath
    bldDir = dircfg^.field @"bld"
    gendDepDir :: FilePath
    gendDepDir = bldDir</>"gendDep"
    clangCDBDir :: FilePath
    clangCDBDir = bldDir</>"clangCDB"

    buildFn :: FilePath -> FilePath -> FilePath -> Action ()
    buildFn out dep clangCDB = do
      liftIO $ forM_ (takeDirectory <$> [out, dep, clangCDB]) (createDirectoryIfMissing True)

      let src = (dircfg^.field @"src")</>dropKnownDirname (dircfg^.field @"bld") out-<.>"cpp"
      need [src]

      let command =
            [cfg^.field @"compiler"] ++
            (cfg^.field @"flags".field @"common") ++
            (if cfg^.field @"debug" then cfg^.field @"flags".field @"debug" else cfg^.field @"flags".field @"release") ++
            (("-isystem"++) <$> cppIncludeListingToPaths tcfg (cfg^.field @"includes".field @"noWarn")) ++
            (("-I"++) <$> cppIncludeListingToPaths tcfg (cfg^.field @"includes".field @"warn")) ++
            ["-o", out]

      cmd_ command "-M" "-MF" [dep] [src]
      needMakefileDependencies dep

      cmd_ command "-MJ" [clangCDB] "-c" [src]
      return ()
dircfgForCPPObjBuild :: CPPObjBuildCfg -> DirCfg
dircfgForCPPObjBuild cfg =
  case cfg^.field @"dircfgOverride" of
    Just x -> x
    Nothing -> over (field @"bld") (</> "cpp_o") (dircfgForCPPTarget $ cfg^.field @"target")


data CPPObjSourceCfg = CPPObjSourceCfg {
  fromInTarget :: [FilePath],
  fromCPPLinkBuild :: [CPPLinkBuildCfg],
  fromWorld :: [FilePath],
  fromUnportablePath :: [FilePath],
  fromCPPObjBuild :: [CPPObjBuildCfg]
} deriving (Generic, Default, Show)
cppObjSourceCfgToPaths :: CPPTargetCfg -> CPPObjSourceCfg -> IO [FilePath]
cppObjSourceCfgToPaths tcfg cfg = do
  cppObjBuildOuts <- mapM findCPPObjectBuildOuts (cfg^.field @"fromCPPObjBuild")
  return . join $
    [(tdircfg^.field @"src"</>) <$> (cfg^.field @"fromInTarget")] ++
    (objPathsForLinkBuild <$> (cfg^.field @"fromCPPLinkBuild")) ++
    [(tdircfg^.field @"world"</>) <$> (cfg^.field @"fromWorld")] ++
    [cfg^.field @"fromUnportablePath"] ++
    cppObjBuildOuts
  where
    tdircfg :: DirCfg
    tdircfg = dircfgForCPPTarget tcfg

    objPathsForLinkBuild :: CPPLinkBuildCfg -> [FilePath]
    objPathsForLinkBuild cfg =
      let dircfg = dircfgForCPPLinkBuild cfg
      in (dircfg^.field @"bld" </>) <$> cfg^.field @"target".field @"publicCPPObjs"


data CPPLibSearchPathCfg = CPPLibSearchPathCfg {
  fromWorld :: [FilePath],
  fromUnportablePath :: [FilePath]
} deriving (Generic, Default, Show)
cppLibSearchPathCfgToPaths :: CPPTargetCfg -> CPPLibSearchPathCfg -> [FilePath]
cppLibSearchPathCfgToPaths tcfg cfg =
  ((tdircfg^.field @"world" </>) <$> (cfg^.field @"fromWorld")) ++ (cfg^.field @"fromUnportablePath")
  where
    tdircfg :: DirCfg
    tdircfg = dircfgForCPPTarget tcfg
data CPPLostLibCfg = CPPLostLibCfg {
  names :: [String],
  searchPath :: CPPLibSearchPathCfg
} deriving (Generic, Default, Show)

data CPPLinkBuildCfg = CPPLinkBuildCfg {
  name :: String,
  target :: CPPTargetCfg,
  dircfgOverride :: Maybe DirCfg,
  objs :: CPPObjSourceCfg,

  lostLibs :: CPPLostLibCfg,
  linker :: String,
  debug :: Bool,
  flags :: CPPFlagCfg
} deriving (Generic, Show)
instance Default CPPLinkBuildCfg where
  def = CPPLinkBuildCfg {
      name = "dist",
      target = def,
      dircfgOverride = def,
      objs = def,

      lostLibs = def,
      linker = "clang++",
      debug = True,
      flags = defaultCPPLinkBuildFlagCfg
    }

defaultCPPLinkBuildFlagCfg :: CPPFlagCfg
defaultCPPLinkBuildFlagCfg =
  CPPFlagCfg {
    common = [],
    debug = [],
    release = []
  }

genCPPLinkBuildRules :: CPPLinkBuildCfg -> Rules ()
genCPPLinkBuildRules cfg =
  getCPPLinkPrimaryBuildOut cfg %> \out -> do
    srcs <- liftIO $ cppObjSourceCfgToPaths tcfg (cfg^.field @"objs")
    need srcs

    let command =
          [cfg^.field @"linker"] ++
          (cfg^.field @"flags".field @"common") ++
          (if cfg^.field @"debug" then cfg^.field @"flags".field @"debug" else cfg^.field @"flags".field @"release") ++
          (("-L"++) <$> (cppLibSearchPathCfgToPaths tcfg $ cfg^.field @"lostLibs".field @"searchPath")) ++
          (("-l"++) <$> (cfg^.field @"lostLibs".field @"names")) ++
          ["-o", out]

    cmd_ command srcs
    return ()
  where
    tcfg :: CPPTargetCfg
    tcfg = cfg^.field @"target"

    dircfg :: DirCfg
    dircfg = dircfgForCPPLinkBuild cfg
dircfgForCPPLinkBuild :: CPPLinkBuildCfg -> DirCfg
dircfgForCPPLinkBuild cfg =
  case cfg^.field @"dircfgOverride" of
    Just x -> x
    Nothing -> over (field @"bld") (</> "cpp_bin") (dircfgForCPPTarget $ cfg^.field @"target")

getCPPLinkPrimaryBuildOut :: CPPLinkBuildCfg -> FilePath
getCPPLinkPrimaryBuildOut cfg = (dircfgForCPPLinkBuild cfg)^.field @"bld"</>cfg^.field @"name"

getCPPLinkBuildOuts :: CPPLinkBuildCfg -> [FilePath]
getCPPLinkBuildOuts cfg =
  [getCPPLinkPrimaryBuildOut cfg]
  where
    dircfg :: DirCfg
    dircfg = dircfgForCPPLinkBuild cfg


linkToDistRules :: FilePath -> FilePath -> Rules ()
linkToDistRules inp out =
  linkToDistRules' inp out (\_ -> return ())

linkToDistRules' :: FilePath -> FilePath -> (FilePath -> Action ()) -> Rules ()
linkToDistRules' inp out act =
  out %> \_ -> do
    cwd <- liftIO $ getCurrentDirectory
    need [inp]
    cmd_ "ln" "-sf" [cwd</>inp] out
    act out

dropKnownDirname :: FilePath -> FilePath -> FilePath
dropKnownDirname base x = joinPath $ dropDir (splitPath base) (splitPath x)
  where
    dropDir :: [FilePath] -> [FilePath] -> [FilePath]
    dropDir [] xs = xs
    dropDir _ [] = []
    dropDir (b:bs) (x:xs) =
      if x == "." || x == "./" then
        dropDir (b:bs) xs
      else if b == "." || b == "./" then
        dropDir bs (x:xs)
      else
        let diff = x \\ b
        in if diff == "" || diff == "/" then
            dropDir bs xs
          else
            error $ "Could not drop '"++(joinPath (b:bs))++"' from '"++(joinPath (x:xs))++"'\n" ++ show (b:bs) ++ " " ++ show (x:xs)

dedup :: (Eq a, Ord a) => [a] -> [a]
dedup = map head . group . sort
