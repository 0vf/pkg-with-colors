--- src/main.c.orig	2023-08-01 08:25:14 UTC
+++ src/main.c
@@ -61,6 +61,13 @@
 
 #include "pkgcli.h"
 
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+#define COLOR_BLUE "\033[34m"
+
 /* Used to define why do we show usage message to a user */
 enum pkg_usage_reason {
 	PKG_USAGE_ERROR,
@@ -165,10 +172,10 @@ usage(const char *conffile, const char *reposdir, FILE
 #else
 #define JAIL_ARG
 #endif
-	fprintf(out, "Usage: pkg [-v] [-d] [-l] [-N] ["JAIL_ARG"-c <chroot path>|-r <rootdir>] [-C <configuration file>] [-R <repo config dir>] [-o var=value] [-4|-6] <command> [<args>]\n");
+	fprintf(out, COLOR_YELLOW BOLD "Usage: " RESET BOLD "pkg [-v] [-d] [-l] [-N] ["JAIL_ARG"-c <chroot path>|-r <rootdir>] [-C <configuration file>] [-R <repo config dir>] [-o var=value] [-4|-6] <command> [<args>]\n" RESET);
 	if (reason == PKG_USAGE_HELP) {
-		fprintf(out, "Global options supported:\n");
-		fprintf(out, "\t%-15s%s\n", "-d", "Increment debug level");
+		fprintf(out, COLOR_YELLOW BOLD "Global options supported:\n" RESET);
+		fprintf(out, COLOR_BLUE "\t%-15s%s\n", "-d", "Increment debug level");
 #ifdef HAVE_LIBJAIL
 		fprintf(out, "\t%-15s%s\n", "-j", "Execute pkg(8) inside a jail(8)");
 #endif
@@ -181,35 +188,35 @@ usage(const char *conffile, const char *reposdir, FILE
 		fprintf(out, "\t%-15s%s\n", "-N", "Test if pkg(8) is activated and avoid auto-activation");
 		fprintf(out, "\t%-15s%s\n", "-o", "Override configuration option from the command line");
 		fprintf(out, "\t%-15s%s\n", "-4", "Only use IPv4");
-		fprintf(out, "\t%-15s%s\n", "-6", "Only use IPv6");
-		fprintf(out, "\nCommands supported:\n");
+		fprintf(out, "\t%-15s%s\n", "-6", "Only use IPv6" RESET);
+		fprintf(out, COLOR_YELLOW BOLD "\nCommands supported:\n" RESET);
 
 		for (i = 0; i < cmd_len; i++)
-			fprintf(out, "\t%-15s%s\n", cmd[i].name, cmd[i].desc);
+			fprintf(out, COLOR_BLUE  "\t%-15s%s\n" RESET, cmd[i].name, cmd[i].desc);
 
 		if (!pkg_initialized() && pkg_ini(conffile, reposdir, 0) != EPKG_OK)
-			errx(EXIT_FAILURE, "Cannot parse configuration file!");
+			errx(EXIT_FAILURE, COLOR_RED BOLD "Cannot parse configuration file!" RESET);
 
 		plugins_enabled = pkg_object_bool(pkg_config_get("PKG_ENABLE_PLUGINS"));
 
 		if (plugins_enabled) {
 			if (pkg_plugins_init() != EPKG_OK)
-				errx(EXIT_FAILURE, "Plugins cannot be loaded");
+				errx(EXIT_FAILURE, COLOR_RED BOLD "Plugins cannot be loaded" RESET);
 
-			fprintf(out, "\nCommands provided by plugins:\n");
+			fprintf(out, COLOR_YELLOW BOLD "\nCommands provided by plugins:\n" RESET);
 
 			tll_foreach(plugins, it) {
-				fprintf(out, "\t%-15s%s\n", it->item->name,
+				fprintf(out, BOLD "\t%-15s%s\n" RESET, it->item->name,
 				    it->item->desc);
 			}
 		}
-		fprintf(out, "\nFor more information on the different commands"
-					" see 'pkg help <command>'.\n");
+		fprintf(out, BOLD "\nFor more information on the different commands"
+					" see 'pkg help <command>'.\n" RESET);
 		exit(EXIT_SUCCESS);
 	}
 
 out:
-	fprintf(out, "\nFor more information on available commands and options see 'pkg help'.\n");
+	fprintf(out, BOLD "\nFor more information on available commands and options see 'pkg help'.\n" RESET);
 	exit(EXIT_FAILURE);
 }
 
@@ -270,15 +277,15 @@ exec_help(int argc, char **argv)
 	all_aliases = pkg_config_get("ALIAS");
 	while ((alias = pkg_object_iterate(all_aliases, &it))) {
 		if (strcmp(argv[1], pkg_object_key(alias)) == 0) {
-			printf("`%s` is an alias to `%s`\n", argv[1], pkg_object_string(alias));
+			printf(COLOR_BLUE BOLD "`%s` " RESET BOLD "is an alias to " COLOR_BLUE BOLD "`%s`\n" RESET, argv[1], pkg_object_string(alias));
 			return (0);
 		}
 	}
 
 	/* Command name not found */
-	warnx("'%s' is not a valid command.\n", argv[1]);
+	warnx(COLOR_BLUE BOLD "'%s' " COLOR_YELLOW "is not a valid command.\n" RESET, argv[1]);
 
-	fprintf(stderr, "See 'pkg help' for more information on the commands.\n");
+	fprintf(stderr, COLOR_YELLOW BOLD "See " COLOR_BLUE "'pkg help' " COLOR_YELLOW "for more information on the commands.\n" RESET);
 
 	return (EXIT_FAILURE);
 }
@@ -744,8 +751,8 @@ main(int argc, char **argv)
 		errx(EXIT_FAILURE, "register pkg_shutdown() to run at exit");
 
 	if (jail_str == NULL && !pkg_compiled_for_same_os_major())
-		warnx("Warning: Major OS version upgrade detected.  Running "
-		    "\"pkg bootstrap -f\" recommended");
+		warnx(COLOR_YELLOW BOLD "Warning: Major OS version upgrade detected.  Running "
+		    "\"pkg bootstrap -f\" recommended" RESET);
 
 
 	plugins_enabled = pkg_object_bool(pkg_config_get("PKG_ENABLE_PLUGINS"));
@@ -785,26 +792,26 @@ main(int argc, char **argv)
 
 	if (argc >= 1 && strcmp(argv[0], "bootstrap") == 0) {
 		if (argc == 1) {
-			printf("pkg(8) already installed, use -f to force.\n");
+			printf(COLOR_GREEN BOLD "pkg(8) already installed, use -f to force.\n" RESET);
 			exit(EXIT_SUCCESS);
 		} else if (argc == 2 && strcmp(argv[1], "-f") == 0) {
 			if (access("/usr/sbin/pkg", R_OK) == 0) {
 				/* Only 10.0+ supported 'bootstrap -f' */
 #if __FreeBSD_version < 1000502
-				printf("Execute these steps to rebootstrap"
+				printf(COLOR_YELLOW BOLD "Execute these steps to rebootstrap"
 				     " pkg(8):\n");
-				printf("# pkg delete -f pkg\n");
-				printf("# /usr/sbin/pkg -v\n");
+				printf(RESET COLOR_BLUE "# pkg delete -f pkg\n");
+				printf("# /usr/sbin/pkg -v\n" RESET);
 				exit(EXIT_SUCCESS);
 #endif
-				printf("pkg(8) is already installed. Forcing "
-				    "reinstallation through pkg(7).\n");
+				printf(COLOR_YELLOW BOLD "pkg(8) is already installed. Forcing "
+				    "reinstallation through pkg(7).\n" RESET);
 				execl("/usr/sbin/pkg", "pkg", "bootstrap",
 				    "-f", NULL);
 				/* NOTREACHED */
 			} else
-				errx(EXIT_FAILURE, "pkg(7) bootstrapper not"
-				    " found at /usr/sbin/pkg.");
+				errx(EXIT_FAILURE, COLOR_RED BOLD "pkg(7) bootstrapper not"
+				    " found at /usr/sbin/pkg." RESET);
 		}
 	}
 
