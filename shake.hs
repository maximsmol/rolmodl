import Development.Shake
import Development.Shake.Command
import Development.Shake.FilePath
import Development.Shake.Util

import Data.List
import System.Directory
import qualified System.Directory as D
import System.Environment
import Control.Monad
import System.IO
import Control.Monad.State

dropDir :: FilePath -> FilePath -> FilePath
dropDir x base = joinPath $ dropDir' (splitPath x) (splitPath base)
  where
    dropDir' :: [FilePath] -> [FilePath] -> [FilePath]
    dropDir' xs [] = xs
    dropDir' (x:xs) (b:bs) =
      let diff = x \\ b
      in if diff == "" || diff == "/" then
          dropDir' xs bs
        else
          error $ "could not drop '"++(joinPath (b:bs))++"' from '"++(joinPath (x:xs))++"'"

listDirRec :: FilePath -> IO [FilePath]
listDirRec path = do
  isDir <- D.doesDirectoryExist path
  if isDir then do
    files <- ((path </>) <$>) <$> listDirectory path
    concat <$> traverse listDirRec files
  else
    return [path]

shakeOptions' :: ShakeOptions
shakeOptions' = shakeOptions{
  shakeFiles = "bld"</>"shake"</>"db",
  shakeReport = (("bld"</>"rep") </>) <$> [
    "t"<.>"trace", "h"<.>"html"
  ],
  shakeLint = Just LintBasic,
  shakeTimings = True
}

main :: IO ()
main = shakeArgs shakeOptions' $ do
  let path_in = "src"
  let path_out = "bld"
  let path_dep = path_out</>"dep"
  let path_compdb = path_out</>"compdb"

  let path_libin     = path_in    </>"lib"
  let path_libout    = path_out   </>"lib"
  let path_libdep    = path_dep   </>"lib"
  let path_libcompdb = path_compdb</>"lib"

  let path_tstin     = path_in    </>"tst"
  let path_tstout    = path_out   </>"tst"
  let path_tstdep    = path_dep   </>"tst"
  let path_tstcompdb = path_compdb</>"tst"

  liftIO $ createDirectoryIfMissing True (path_out</>"rep")

  compilerFromEnv <- liftIO $ lookupEnv "CXX"
  let compiler =
        case compilerFromEnv of
          Just x -> if null x then "clang++" else x
          Nothing -> "clang++"

  let exec = "dst"</>"dbg"</>"exec"
  -- let static_lib = "dst"</>"dbg"</>"rolmodl.so" -- no support on macos
  let dynamic_lib = "dst"</>"dbg"</>"librolmodl.dylib"
  let compdb = path_out</>"compile_commands.json"

  want [exec, compdb]

  dynamic_lib %> \out -> do
    let srcs = [
          path_libout</>"Win"<.>"o",
          path_libout</>"Ren"<.>"o",
          path_libout</>"Base"<.>"o",
          path_libout</>"Kb"<.>"o",
          path_libout</>"Mouse"<.>"o",
          path_libout</>"PixelFmt"<.>"o",
          path_libout</>"Tex"<.>"o",
          path_libout</>"Event"<.>"o"
          ]
    need srcs

    let libFlags = ("-l"++) <$> ["sdl2"]

    () <- cmd "clang++" "-shared" "-install_name" "@rpath/librolmodl.dylib" "-O0" "-o" [out] libFlags "-L/usr/local/opt/llvm/lib" srcs
    return ()

  exec %> \out -> do
    let srcs = [
          path_tstout</>"main"<.>"o"
          ]
    need srcs
    need [dynamic_lib]

    let libFlags = ("-l"++) <$> ["sdl2", "rolmodl"]

    () <- cmd "clang++" "-rpath" "@executable_path" "-O0" "-o" [out] libFlags ("-L"++takeDirectory dynamic_lib) "-L/usr/local/opt/llvm/lib" srcs
    return ()

  compdb %> \out -> do
    files <- liftIO $ listDirRec path_compdb
    () <- cmd Shell ["sed", "-e", "'1s/^/[\\\n/'", "-e", "'$s/,$/\\\n]/'"] files [">", out]
    return ()

  let cppRoutine = \src dep out compdb -> do
      --  "-Ofast"
      let diagFlags = ["-fcolor-diagnostics"]
      let warnFlags = ["-Weverything", "-Wno-c++98-compat", "-Wno-c++98-c++11-compat", "-Wno-c++98-c++11-compat-pedantic", "-Wno-c99-extensions", "-Wno-c++98-c++11-c++14-compat", "-Wno-padded"]
      let outputFlags = ["-o", out]
      let includeFlags = ("-isystem"++) <$> ["/usr/local/include/SDL2", "/usr/local/opt/llvm/include"]
      let otherFlags = ["-std=c++1z"]
      let command = [compiler, "-O0"] ++ diagFlags ++ warnFlags ++ outputFlags ++ includeFlags ++ otherFlags

      () <- cmd command "-M" "-MF" [dep] [src]
      needMakefileDependencies dep

      () <- cmd command "-MJ" [compdb] "-c" [src]
      return ()

  [path_libout<//>"*"<.>"o", path_libdep<//>"*"<.>"dep", path_libcompdb<//>"*"<.>"json"] &%> \[out, dep, compdb] -> do
    let src = path_libin</>(dropDir out path_libout)-<.>"cpp"
    cppRoutine src dep out compdb

  [path_tstout<//>"*"<.>"o", path_tstdep<//>"*"<.>"dep", path_tstcompdb<//>"*"<.>"json"] &%> \[out, dep, compdb] -> do
    let src = path_tstin</>(dropDir out path_tstout)-<.>"cpp"
    cppRoutine src dep out compdb

  phony "dbg" $ do
    return ()
