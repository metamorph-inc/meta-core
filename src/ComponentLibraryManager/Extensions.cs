using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using GME.MGA;
using System.IO;

namespace META
{
    public static class ComponentLibraryManagerExtensions
    {
        /// <summary>
        /// Given a component, ensures that the component has a backend folder
        /// for storing resources. Ensures that the Component has an AVMID unique
        /// to the project. Creates a folder if necessary.
        /// </summary>
        /// <param name="component"></param>
        /// <param name="pathConvention">The desired convention for the path.</param>
        /// <param name="ProjectDirectory">Directory in which component files reside. Defaults to project directory of <paramref name="component"/></param>
        /// <returns>The path of the Component folder, according to the convention provided</returns>
        public static String GetDirectoryPath(this CyPhy.Component component, ComponentLibraryManager.PathConvention pathConvention = ComponentLibraryManager.PathConvention.REL_TO_PROJ_ROOT, string ProjectDirectory = null)
        {
            return ComponentLibraryManager.GetComponentFolderPath(component, pathConvention, ProjectDirectory: ProjectDirectory);
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
        public static String GetDirectoryPath(this CyPhy.ComponentAssembly assembly, ComponentLibraryManager.PathConvention pathConvention = ComponentLibraryManager.PathConvention.REL_TO_PROJ_ROOT, string ProjectDirectory = null)
        {
            var relPath = ComponentLibraryManager.EnsureComponentAssemblyFolder(assembly, ProjectDirectory);
            switch (pathConvention)
            {
                case ComponentLibraryManager.PathConvention.REL_TO_PROJ_ROOT:
                    return relPath;
                case ComponentLibraryManager.PathConvention.ABSOLUTE:
                    return Path.Combine(GetRootDirectoryPath(assembly.Impl.Project), relPath);
                default:
                    throw new ArgumentOutOfRangeException(String.Format("Path convention of {0} is not supported", 
                                                                        pathConvention.ToString()));
            }
        }

        /// <summary>
        /// Given a DomainModel object, find the path to the file resource.
        /// This will traverse the UsesResource connection to a Resource object,
        /// then return that Resource object's path.
        /// </summary>
        /// <param name="path">The path to the resource, relative to the component's resource folder</param>
        /// <param name="pathConvention"></param>
        /// <returns>True if the resource was found</returns>
        public static bool TryGetResourcePath(this CyPhy.DomainModel domainModel, out string path, ComponentLibraryManager.PathConvention pathConvention = ComponentLibraryManager.PathConvention.REL_TO_COMP_DIR)
        {
            return ComponentLibraryManager.TryGetResourcePath(domainModel, out path, pathConvention);
        }

        /// <summary>
        /// Given a CyPhy MGA project, return the root directory of the project
        /// </summary>
        /// <param name="project"></param>
        /// <returns>The absolute path to the project's root directory</returns>
        public static String GetRootDirectoryPath(this IMgaProject project)
        {
            return ComponentLibraryManager.GetProjectRootPath(project);
        }

    }
}
