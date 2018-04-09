using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using AVM.DDP;
using GME.CSharp;
using GME.MGA;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace META
{
    public static class ComponentLibraryManager
    {
        private static String ComponentsFolderName = "components";

        public enum PathConvention
        {
            REL_TO_COMP_DIR,
            REL_TO_PROJ_ROOT,
            ABSOLUTE
        };

        /// <summary>
        /// Given a component, ensures that is has an AVMID unique to its project.
        /// If it has an AVMID, and it is unique, it will not be changed.
        /// Otherwise, a new one will be assigned.
        /// </summary>
        /// <param name="component"></param>
        /// <returns>The resulting AVMID of the Component</returns>
        public static String EnsureAVMID(CyPhy.Component component)
        {
            var mp_GmeProject = component.Impl.Project;

            Boolean b_AssignNewAVMID = false;

            // If this component doesn't have an AVM ID already, we will assign one.
            if (String.IsNullOrWhiteSpace(component.Attributes.AVMID))
            {
                b_AssignNewAVMID = true;
            }
            // If component is not an archetype (i.e.: not defined in a Components folder), don't mess with it.
            else if (component.ParentContainer.Kind != "Components")
            {
                b_AssignNewAVMID = false;
            }
            else
            {
                // Already has an AVMID -- Scan project for conflicts
                MgaFilter filter = mp_GmeProject.CreateFilter();
                filter.Kind = "Component";

                // Only find Components whose parent folders are "Components" folders
                foreach (var item in mp_GmeProject.AllFCOs(filter)
                                                  .Cast<MgaFCO>()
                                                  .Where(i => i.ParentFolder != null
                                                               && i.ParentFolder.MetaBase.Name == "Components"))
                {
                    var c_item = CyPhyClasses.Component.Cast(item);
                    if (c_item.ID != component.ID && c_item.Attributes.AVMID == component.Attributes.AVMID)
                        b_AssignNewAVMID = true;
                }
            }
            if (b_AssignNewAVMID)
                component.Attributes.AVMID = Guid.NewGuid().ToString("D");

            return component.Attributes.AVMID;
        }

        /// <summary>
        /// Given a component assembly, ensures that the component assembly has a backend folder
        /// for storing resources. Creates a folder if necessary.
        /// </summary>
        /// <param name="ProjectDirectory">Directory in which the /designs/ folder resides. Defaults to project directory of <paramref name="componentAssembly"/></param>
        /// <returns>The path of the ComponentAssembly folder, relative to the project root</returns>
        public static String EnsureComponentAssemblyFolder(CyPhy.ComponentAssembly componentAssembly, string ProjectDirectory = null)
        {
            var mp_MgaProject = componentAssembly.Impl.Project;
            String p_ProjectRoot = ProjectDirectory ?? mp_MgaProject.GetRootDirectoryPath();

            if (string.IsNullOrEmpty(componentAssembly.Attributes.Path))
            {
                componentAssembly.Attributes.Path = GetRandomComponentAssemblyDir();
            }
            Directory.CreateDirectory(Path.Combine(p_ProjectRoot, componentAssembly.Attributes.Path));

            return componentAssembly.Attributes.Path;
        }

        public static string GetRandomComponentAssemblyDir()
        {
            return "designs\\" + Path.GetRandomFileName().Replace(".", "");
        }

        /// <summary>
        /// Given a component, ensures that the component has a backend folder
        /// for storing resources. Ensures that the Component has an AVMID unique
        /// to the project.  Creates a folder if necessary.
        /// </summary>
        /// <param name="component"></param>
        /// <param name="ProjectDirectory">Directory in which the /components/ folder resides. Defaults to project directory of <paramref name="component"/></param>
        /// <returns>The path of the Component folder, relative to the project root</returns>
        public static String EnsureComponentFolder(CyPhy.Component component, string ProjectDirectory = null)
        {
            Boolean bIsArchetype = (component.ParentContainer.Kind == "Components");

            String path = "";
            EnsureAVMID(component);

            var mp_MgaProject = component.Impl.Project;
            String p_ProjectRoot = ProjectDirectory ?? mp_MgaProject.GetRootDirectoryPath();
            // META-2640 use full path instead of relative project path
            if (p_ProjectRoot == "")
            {
                p_ProjectRoot = ".";
            }
            p_ProjectRoot = Path.GetFullPath(p_ProjectRoot);
            var lastChar = p_ProjectRoot.Last();
            if (lastChar != '\\' && lastChar != '/')
                p_ProjectRoot += '\\';

            // Try to get a path from the CyPhy model.
            // If that fails, check the manifest.
            Boolean bHasFolderSpecified = false;
            if (!String.IsNullOrWhiteSpace(component.Attributes.Path))
            {
                bHasFolderSpecified = true;
                path = component.Attributes.Path;

                // Strip filename if necessary
                if (path.EndsWith(".acm"))
                    path = Path.GetDirectoryName(path);
                else if (path.EndsWith("ComponentData.xml"))
                    path = Path.GetDirectoryName(path);
            }
            else
            {
                // If a project file already exists, this will load it.
                MetaAvmProject proj = MetaAvmProject.Create(p_ProjectRoot, component.Impl.Project, null, GMEConsole.CreateFromProject(mp_MgaProject));

                // Check to see if this component has a folder already.
                var c_CompEntryInProjectFile = proj.Project.Components.Find(x => x.avmid == component.Attributes.AVMID);
                if (c_CompEntryInProjectFile != null)
                {
                    bHasFolderSpecified = true;
                    path = Path.GetDirectoryName(c_CompEntryInProjectFile.modelpath);
                }
            }

            bHasFolderSpecified = bHasFolderSpecified
                                  && !PathContainsIllegalChar(path);

            if (bHasFolderSpecified && !Directory.Exists(Path.Combine(p_ProjectRoot, path)))
            {
                Directory.CreateDirectory(Path.Combine(p_ProjectRoot, path));
            }
            // Only create new folders for archetypes
            else if (!bHasFolderSpecified && bIsArchetype)
            {
                var absPath = CreateComponentFolder(mp_MgaProject);
                path = MakeRelativePath(p_ProjectRoot, absPath);
                path = path.Replace('\\', '/') + '/';
            }

            // Strip any leading path character
            if (path.Length >= 2 && (path.Substring(0, 2) == "./" || path.Substring(0, 2) == ".\\"))
                path = path.Remove(0, 2);

            // Store the path in the component's attribute for faster retrieval next time.
            component.Attributes.Path = path;

            return path;
        }

        private static bool PathContainsIllegalChar(string path)
        {
            var invalidChars = Path.GetInvalidPathChars().ToList();
            invalidChars.Add('?');
            invalidChars.Add('*');
            invalidChars.Add(':');

            return path.IndexOfAny(invalidChars.ToArray()) != -1
                || path.EndsWith(" ");
        }

        /// <summary>
        /// Given a component, ensures that the component has a backend folder
        /// for storing resources. Ensures that the Component has an AVMID unique
        /// to the project. Creates a folder if necessary.
        /// </summary>
        /// <param name="component"></param>
        /// <param name="pathConvention">The desired convention for the path.</param>
        /// <param name="ProjectDirectory">Directory in which component files reside. Defaults to project directory of <paramref name="component"/></param>
        /// <returns>The path of the Component folder, according to the convention provided</returns>
        public static String GetComponentFolderPath(CyPhy.Component component, PathConvention pathConvention = PathConvention.REL_TO_PROJ_ROOT, string ProjectDirectory = null)
        {
            var path_RelativeToProjRoot = EnsureComponentFolder(component, ProjectDirectory: ProjectDirectory);
            if (pathConvention == PathConvention.REL_TO_PROJ_ROOT)
            {
                return path_RelativeToProjRoot;
            }
            else if (pathConvention == PathConvention.ABSOLUTE)
            {
                var path_ProjRootDir = ProjectDirectory ?? component.Impl.Project.GetRootDirectoryPath();
                return Path.Combine(path_ProjRootDir, path_RelativeToProjRoot);
            }
            else
            {
                throw new NotSupportedException(String.Format("{0} doesn't support PathConvention of type {1}",
                                                                System.Reflection.MethodBase.GetCurrentMethod().Name,
                                                                pathConvention.ToString()));
            }
        }


        /// <summary>
        /// Creates a unique folder, used for storing component resources, under the project's "components" folder.
        /// This operation DOES NOT assign the folder to any component;
        /// Consider using EnsureComponentDirectory(...) for this purpose.
        /// </summary>
        /// <param name="projectPath">The CyPhy MGA project</param>
        /// <returns>The absolute path of the new folder</returns>
        public static String CreateComponentFolder(IMgaProject project)
        {
            String p_ComponentsDir = Path.Combine(project.GetRootDirectoryPath(), ComponentsFolderName);

            // No entry exists. Make a folder and also an entry.
            String s_RandomFolderName = Path.GetFileNameWithoutExtension(Path.GetRandomFileName());
            String p_NewComponentDir = Path.Combine(p_ComponentsDir, s_RandomFolderName);

            int maxFolders = 0;

            while (File.Exists(p_NewComponentDir) ||
                    Directory.Exists(p_NewComponentDir))
            {
                if (maxFolders++ > 2000000)
                {
                    throw new Exception(
                        string.Format("Number of tries ({0}) to create an output folder exceeded in {0}. ",
                            maxFolders,
                            p_ComponentsDir));
                }

                s_RandomFolderName = Path.GetFileNameWithoutExtension(Path.GetRandomFileName());

                p_NewComponentDir = Path.Combine(
                    p_ComponentsDir,
                    s_RandomFolderName);
            }
            Directory.CreateDirectory(p_NewComponentDir);

            return p_NewComponentDir;
        }

        /// <summary>
        /// Creates a relative path from one file or folder to another.
        /// </summary>
        /// <param name="fromPath">Contains the directory that defines the start of the relative path.</param>
        /// <param name="toPath">Contains the path that defines the endpoint of the relative path.</param>
        /// <param name="dontEscape">Boolean indicating whether to add uri safe escapes to the relative path</param>
        /// <returns>The relative path from the start directory to the end path.</returns>
        public static string MakeRelativePath(string fromPath, string toPath)
        {
            Contract.Requires(string.IsNullOrEmpty(fromPath) == false);
            Contract.Requires(string.IsNullOrEmpty(toPath) == false);

            Uri fromUri = new Uri(Path.GetFullPath(fromPath));
            Uri toUri = new Uri(Path.GetFullPath(toPath));

            Uri relativeUri = fromUri.MakeRelativeUri(toUri);
            string relativePath = Uri.UnescapeDataString(relativeUri.ToString());

            return relativePath.Replace('/', Path.DirectorySeparatorChar);
        }


        /// <summary>
        /// Given a DomainModel object, find the path to the file resource.
        /// This will traverse the UsesResource connection to a Resource object,
        /// then return that Resource object's path.
        /// </summary>
        /// <param name="domainModel"></param>
        /// <param name="path">The path to the resource, relative to the component's resource folder</param>
        /// <param name="pathConvention"></param>
        /// <returns>True if the resource was found</returns>
        public static bool TryGetResourcePath(CyPhy.DomainModel domainModel, out string path, PathConvention pathConvention = PathConvention.REL_TO_COMP_DIR)
        {
            if (false == (domainModel.ParentContainer is CyPhy.Component)
                && false == (domainModel.ParentContainer is CyPhy.ComponentAssembly)
                && (pathConvention == PathConvention.REL_TO_PROJ_ROOT || pathConvention == PathConvention.ABSOLUTE))
            {
                throw new NotSupportedException(String.Format("{0} doesn't support PathConvention of type {1} if domainModel.ParentContainer is not a CyPhy.Component",
                                                                System.Reflection.MethodBase.GetCurrentMethod().Name,
                                                                pathConvention.ToString()));
            }

            // Starting with the DomainModel object, we'll find a Resource object to which it has a relation.
            // Then we'll return the path of that Resource object.

            List<CyPhy.Resource> l_Resources = new List<CyPhy.Resource>();
            foreach (CyPhy.UsesResource ur in domainModel.DstConnections.UsesResourceCollection)
                l_Resources.Add(ur.DstEnds.Resource);
            foreach (CyPhy.UsesResource ur in domainModel.SrcConnections.UsesResourceCollection)
                l_Resources.Add(ur.SrcEnds.Resource);

            path = "";

            // If no Resources found, return false.
            if (l_Resources.Count == 0)
            {
                return false;
            }

            // At least one Resource was found, so return the path of the first.
            Func<string> getDirPath = delegate
            {
                if (domainModel.ParentContainer is CyPhy.Component)
                {
                    return GetComponentFolderPath((CyPhy.Component)domainModel.ParentContainer);
                }
                else
                {
                    return ((CyPhy.ComponentAssembly)domainModel.ParentContainer).Attributes.Path;
                }
            };
            var resourcePath = l_Resources.FirstOrDefault().Attributes.Path;
            if (pathConvention == PathConvention.REL_TO_COMP_DIR)
            {
                path = resourcePath;
            }
            else if (pathConvention == PathConvention.REL_TO_PROJ_ROOT)
            {
                string compDirPath = getDirPath();
                path = Path.Combine(compDirPath, resourcePath);
            }
            else if (pathConvention == PathConvention.ABSOLUTE)
            {
                string compDirPath = getDirPath();
                var projRootPath = domainModel.Impl.Project.GetRootDirectoryPath();
                path = Path.Combine(projRootPath, compDirPath, resourcePath);
            }
            else
            {
                throw new NotSupportedException(String.Format("{0} doesn't support PathConvention of type {1}",
                                                                System.Reflection.MethodBase.GetCurrentMethod().Name,
                                                                pathConvention.ToString()));
            }

            return true;
        }

        /// <summary>
        /// Retrieves the path to the component's original ACM file, stored when it was imported.
        /// Note that ANY ACM file found will be flagged as a match, even if it has been updated for some reason.
        /// </summary>
        /// <param name="component"></param>
        /// <param name="path">The path to the ACM file, according to the PathConvention</param>
        /// <param name="pathConvention"></param>
        /// <param name="ProjectDirectory">Directory in which component files reside. Defaults to project directory of <paramref name="component"/></param>
        /// <returns>True only if an ACM was found</returns>
        public static bool TryGetOriginalACMFilePath(CyPhy.Component component, out string path, PathConvention pathConvention = PathConvention.REL_TO_COMP_DIR, string ProjectDirectory = null)
        {
            var componentPath = component.GetDirectoryPath(pathConvention, ProjectDirectory: ProjectDirectory);

            var acmFiles = Directory.EnumerateFiles(componentPath, "*.acm", SearchOption.TopDirectoryOnly);
            if (acmFiles.Any())
            {
                path = Path.Combine(componentPath, acmFiles.First());
                return true;
            }
            else if (File.Exists(Path.Combine(componentPath, "ComponentData.xml")))
            {
                path = Path.Combine(componentPath, "ComponentData.xml");
                return true;
            }

            path = "";
            return false;
        }

        /// <summary>
        /// Get the absolute path to the root directory of a CyPhy project.
        /// </summary>
        /// <param name="project"></param>
        /// <returns></returns>
        public static String GetProjectRootPath(IMgaProject project)
        {
            return MgaExtensions.MgaExtensions.GetProjectDirectoryPath(project);
        }
    }
}
