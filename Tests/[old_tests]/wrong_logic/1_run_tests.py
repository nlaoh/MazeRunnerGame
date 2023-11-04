import os

# Replace this with the path to your test directory
test_directory = "Tests/wrong_logic/"

# List all files in the directory
test_files = os.listdir(test_directory)

# Filter for .input files
test_names = [file.split(".input")[0] for file in test_files if file.endswith(".input")]

for test_name in test_names:
    # Build input and output file paths
    input_file = os.path.join(test_directory, f"{test_name}.input")
    output_file = os.path.join(test_directory, f"{test_name}.out")
    expected_output_file = os.path.join(test_directory, f"{test_name}.expout")
    
    # To run the program
    run_command = f"./mazeRunner -testmode < {input_file} > {output_file}"
    
    # To check the output
    diff_command = f"diff -w {expected_output_file} {output_file}"
    
    # To delete the output file
    delete_command = f"rm {output_file}"
    
    print(f"Running test: {test_name}")
    
    # Execute the commands
    os.system(run_command)
    os.system(diff_command)
    # os.system(delete_command)
    
    print(f"Test {test_name} completed\n")