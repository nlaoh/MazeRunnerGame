import os

# Replace this with the path to your test directory
test_directory = "Tests/validity_tests/build_maze_enhanced"

# List all files in the directory
test_files = os.listdir(test_directory)

# Filter for .input files
test_names = [file.split(".input")[0] for file in test_files if file.endswith(".input")]

# Exit condition
Exit = False

while not Exit:
    print("Available Tests:")
    for i, test_name in enumerate(test_names, start=1):
        print(f"{i}. {test_name}")
    
    # Check input
    CorrectSelection = False
    while not CorrectSelection:
        try: 
            selection = input("Enter the test number or range to run (e.g., '1' for a single test, '1-3,3-4,7' for multiple tests, 'all' to run all tests, or 'exit' to quit): \n")
            if selection == 'all':
                CorrectSelection = True
                selected_tests = test_names
            elif selection == 'exit':
                CorrectSelection = True
                Exit = True
                print("Exiting.")
            else:
                ranges = selection.split(',')
                selected_tests = []

                for r in ranges:
                    components = r.split('-')
                    if len(components) == 2:
                        # Handle range of tests
                        start, end = map(int, components)
                        if 1 <= start <= end <= len(test_names):
                            CorrectSelection = True
                            selected_tests.extend(test_names[start - 1:end])
                        else:
                            print(f"Invalid range: {r}. Please enter a valid range of test numbers.")
                            break
                    else:
                        components = r.split()
                        for component in components:
                            try:
                                test_number = int(component)
                                if 1 <= test_number <= len(test_names):
                                    CorrectSelection = True
                                    selected_tests.append(test_names[test_number - 1])
                                else:
                                    print(f"Invalid selection: {component}. Please enter a valid test number.")
                                    break
                            except ValueError:
                                print(f"Invalid input: {component}. Please enter a valid test number, range, 'all', or 'exit.")
                                break

                if len(selected_tests) == len(ranges):
                    CorrectSelection = True
        except ValueError:
            print(f"Invalid input: Please enter a valid test number, range, 'all', or 'exit.")

    if Exit:
        break  
    
    for test_name in selected_tests:
        input_file = os.path.join(test_directory, f"{test_name}.input")
        output_file = os.path.join(test_directory, f"{test_name}.out")
        expected_output_file = os.path.join(test_directory, f"{test_name}.expout")

        run_command = f"./mazeRunner -testmode -enhancements < {input_file} > {output_file}"
        diff_command = f"diff -w {expected_output_file} {output_file}"
        delete_command = f"rm {output_file}"

        print(f"Running test: {test_name}")

        try:
            os.system(run_command)
            os.system(diff_command)
        except Exception as e:
            print(f"Error encountered: {str(e)}")

        # os.system(delete_command)

        print(f"Test {test_name} completed\n")

    CorrectContinue = False
    while not CorrectContinue:
        continue_prompt = input("Do you want to continue running tests? (y/n): ")
        if continue_prompt.lower() == 'y':
            CorrectContinue = True
            CorrectSelection = False
        elif continue_prompt.lower() == 'n':
            CorrectContinue = True
            Exit = True
            print("Exiting.")
        else: 
            print("Invalid input")
